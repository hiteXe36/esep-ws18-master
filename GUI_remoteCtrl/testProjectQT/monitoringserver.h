#ifndef MONITORINGSERVER_H
#define MONITORINGSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTextBrowser>
#include <QLabel>

class MonitoringServer : public QTcpServer
{
    Q_OBJECT

public:
    MonitoringServer(QTextBrowser *log, QLabel *status, QObject *parent = 0);

protected:
    void incomingConnection(qintptr sock) override;

private:
    QTextBrowser *log;
    QLabel *status;
};

#endif // MONITORINGSERVER_H
