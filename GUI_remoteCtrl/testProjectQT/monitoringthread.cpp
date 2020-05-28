#include "monitoringthread.h"

#include <QDebug>

MonitoringThread::MonitoringThread(int sock, QTextBrowser *log, QLabel *status, QObject *parent)
    : QThread(parent), sock(sock), log(log), status(status)
{
}

void MonitoringThread::run()
{
    qDebug() << "Monitoring thread started: acception connections from clients";
    QTcpSocket tcpSocket;
    if (!tcpSocket.setSocketDescriptor(sock)) {
        emit error(tcpSocket.error());
        return;
    }

    tcpSocket.waitForConnected(-1); // no timeout
    qDebug() << "Connection received. Accepting log from clients";
    status->setText("Connected");

    while(tcpSocket.state() == QAbstractSocket::SocketState::ConnectedState) {
        qDebug() << "Waiting for log from client";
        tcpSocket.waitForReadyRead(-1);
        QString logtext(tcpSocket.readLine());
        log->append(logtext.trimmed());
    }

    qDebug() << "MonitoringServer: connection to client closed";
}
