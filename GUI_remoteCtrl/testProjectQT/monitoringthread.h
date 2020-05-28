#ifndef MONITORINGTHREAD_H
#define MONITORINGTHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QTextBrowser>
#include <QLabel>

class MonitoringThread : public QThread
{
    Q_OBJECT

public:
    MonitoringThread(int sock, QTextBrowser *log, QLabel *status, QObject *parent);

    void run() override;

signals:
    void error(QTcpSocket::SocketError socketError);

private:
    int sock;
    QTextBrowser *log;
    QLabel *status;
};

#endif // MONITORINGTHREAD_H
