#include "monitoringserver.h"
#include "monitoringthread.h"

#include <QDebug>

MonitoringServer::MonitoringServer(QTextBrowser *log, QLabel *status, QObject *parent)
    : QTcpServer(parent), log(log), status(status)
{

}

void MonitoringServer::incomingConnection(qintptr sock)
{
    MonitoringThread *thread = new MonitoringThread(sock, log, status, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
    qDebug() << "Monitoring server: listening for new connection!";
    status->setText("Listening for new connection");
}
