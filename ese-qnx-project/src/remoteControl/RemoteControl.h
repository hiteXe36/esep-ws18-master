#ifndef SERVER_H_
#define SERVER_H_

#define	SRV_PORT	(8080)		/**< Default port to use. */
#define	MONITOR_PORT	(8081)	/**< Monitor port to use. */
#define BUFSIZE		(2U)		/**< Size of buffer to receive command from client. */

#include <sys/socket.h>
#include "../servicetools/Channel.h"
/**
 * @brief A remote control for the machine actors.
 * It uses the posix Socket API to create a connection between the machine (Server and Client)
 */
class RemoteControl {
public:
	/**
	 * @brief Constructor for using the default port 8080.
	 * @param dispatcherChannel Channel to send message to dispatcher.
	 */
	RemoteControl(Channel& dispatcherChannel, Channel& toMe);

	/**
	 * @brief Constructor for using given port.
	 * @param dispatcherChannel Channel to send message to dispatcher.
	 * @param port Port to use.
	 */
	RemoteControl(Channel& dispatcherChannel, Channel& toMe, int port);

	/**
	 * @brief overload () operator
	 */
	void operator()();
	virtual ~RemoteControl();
private:
	bool monitoringConnected;

	/**
	 * Create the server on the machine and waiting for the connection.
	 * @param port port to listen to connection.
	 * @param sa properties of the connection.
	 * @param sa_len lenght of sa.
	 */
	int init(int port);
	/**
	 * Waiting for the connection.
	 * @param srv_sock Server Socket.
	 * @param sa properties of the connection.
	 * @param sa_len length of sa.
	 */
	int waitForConnection(int srv_sock, struct sockaddr *sa, socklen_t *sa_len);
	/**
	 * receive command from client and pass to Main-Control
	 * @param client client Socket.
	 */
	void receiveCommand(int client);
	/**
	 * Create the connection to client for monitoring.
	 * @param client client Socket.
	 */
	int connectionForMonitoring(int client);

	char buf[BUFSIZE]; 			/**<receiving buffer */
	int srv_sock_; 				/**<Server Socket */
	Channel& dispatcherChannel;	/**<Channel to dispatcher */
	Channel& _2me;				/**<Channel to RemoteControl */
};

#endif /* SERVER_H_ */
