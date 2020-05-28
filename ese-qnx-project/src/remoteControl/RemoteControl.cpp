#include "RemoteControl.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include "../hal/HAL.h"
#include "../servicetools/Message.h"
#include "../servicetools/enums.h"
#include "../servicetools/Logger.h"

#include <memory>
#include <iostream>
#include <cstring>
#include <cerrno>
#include <cstdlib>
#include <thread>
#include <string>

RemoteControl::RemoteControl(Channel& dispatcherChannel, Channel& toMe) :
		dispatcherChannel(dispatcherChannel), _2me(toMe) {

}

RemoteControl::RemoteControl(Channel& dispatcherChannel, Channel& toMe,
		int port) :
		dispatcherChannel(dispatcherChannel), _2me(toMe) {

}

int RemoteControl::connectionForMonitoring(int client) {
	struct sockaddr_in sa;
	socklen_t sa_len = sizeof(struct sockaddr_in);

	if ((getpeername(client, (sockaddr*) &sa, &sa_len)) < 0) {
		perror("IP-Address from Client");
		exit(1);
	}

	sa.sin_family = AF_INET; /* select IPv4 */
	sa.sin_port = htons(MONITOR_PORT);

	/* print the ipv6 address */
	char addr[256];
	std::string addrStr(inet_ntop(sa.sin_family, &sa.sin_addr.s_addr, addr, sizeof(addr)));
	LOG_WRITE("Got connection from: " + addrStr);

	int monitor;
	if ((monitor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("TCP Socket");
		exit(1);
	}
	if (connect(monitor, (const struct sockaddr *) (&sa), sa_len) < 0) {
		perror("Connect");
		std::cout << errno << std::endl;
		exit(1);
	}
	monitoringConnected = true;
	return monitor;
}

void RemoteControl::operator()() {
	std::cout << "Starting RemoteControl" << std::endl;
	int client = init(SRV_PORT);
	int monitor = connectionForMonitoring(client);
	std::thread receiveThr(&RemoteControl::receiveCommand, this,
			std::ref(client));

	while (monitoringConnected) {
		auto msg = std::make_unique < Message > (_2me.dequeue());
		if (strcmp(msg->payload, "LOG") == 0) {
			std::string msgLog = "Signal: "
					+ Enums::signalToString(msg->signal) + "\n";
			int n;
			if ((n = send(monitor, msgLog.c_str(), strlen(msgLog.c_str()), 0)) < 0) {
				perror("Send LOG");
			}
		} else {
			dispatcherChannel.enqueue(*msg);
		}
	}
	receiveThr.join();
}

RemoteControl::~RemoteControl() {
	// TODO Auto-generated destructor stub
}

int RemoteControl::init(int port) {
	int ret;
	struct sockaddr_in sa;
	socklen_t sa_len = sizeof(struct sockaddr_in);

	sa.sin_family = AF_INET; /* select IPv4 */
	sa.sin_port = htons(SRV_PORT);
	sa.sin_addr.s_addr = INADDR_ANY;
	LOG_WRITE("Socket: Verbindungsaufbau!");
	srv_sock_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (srv_sock_ < 0) {
		throw std::logic_error(
				std::string("Failed to create socket: ")
						+ std::strerror(errno));
	}

	ret = bind(srv_sock_, (struct sockaddr*) &sa, sa_len);
	if (ret < 0) {
		throw std::logic_error(
				std::string("Failed to bind socket: ") + std::strerror(errno));
	}

	ret = listen(srv_sock_, 1);
	if (ret < 0) {
		close(srv_sock_);
		throw std::logic_error(
				std::string("Failed to listen on socket: ")
						+ std::strerror(errno));
	}

	int clientfd = waitForConnection(srv_sock_, (struct sockaddr*) &sa, &sa_len);
	return clientfd;
}

int RemoteControl::waitForConnection(int srv, struct sockaddr *sa,
		socklen_t *sa_len) {
	LOG_WRITE("Wait for connection");
	int client = accept(srv, sa, sa_len);
	if (client < 0) {
		close(srv);
		throw std::logic_error(
				std::string("Failed to accept connection: ")
						+ std::strerror(errno));
	}
	dispatcherChannel.enqueue(*(std::make_unique<Message>(Signal::REMOTE_CONNECTED, Component::DISPATCHER, Component::REMOTE)));
	return client;
}

Component getComponent(int signal) {
	if (signal >= static_cast<int>(Signal::GREEN_LIGHT_ON) && signal <= static_cast<int>(Signal::RED_LIGHT_BLINK_OFF)) {
		return Component::LAMP;
	} else if (signal >= static_cast<int>(Signal::MOTOR_ON) && signal <= static_cast<int>(Signal::MOTOR_STOP)) {
		return Component::MOTOR;
	} else if (signal == static_cast<int>(Signal::GATE_OPEN) || signal == static_cast<int>(Signal::GATE_CLOSE)) {
		return Component::GATE;
	} else {
		std::cerr << "Signal not valid" << std::endl;
		return Component::NULL_COMPONENT;
	}
}

void RemoteControl::receiveCommand(int client) {
	int ret;
	while (true) {
		ret = recv(client, (void *) buf, BUFSIZE, 0);
		if (ret < 0) {
			LOG_WRITE("Error receiving data from client");
		} else {
			LOG_WRITE("Received message from client: " + std::string(buf));
			int zahl = atoi(buf);

			/* wait for Message from Dispatcher */
			Component dest = getComponent(zahl);
			Signal signal = static_cast<Signal>(zahl);
			if (dest == Component::NULL_COMPONENT)
				continue;

			_2me.enqueue(*(std::make_unique<Message>(signal, dest, Component::REMOTE)));
		}
	}
}
