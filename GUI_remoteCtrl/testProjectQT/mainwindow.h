#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "monitoringserver.h"

#include <memory>
#include <thread>

#include <QMainWindow>
#include <QString>
#include <QTcpSocket>
#include <QTcpServer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_stopMotor_clicked();

    void on_openGate_clicked();

    void on_closeGate_clicked();

    void on_greenOn_clicked();

    void on_greenOff_clicked();

    void on_yellowOn_clicked();

    void on_yellowOff_clicked();

    void on_redOn_clicked();

    void on_redOff_clicked();

    void on_motorRight_clicked();

    void on_motorLeft_clicked();

    void on_controlIP_returnPressed();

    void on_monitoringPort_returnPressed();

    void on_motorSpeedSlow_clicked();

    void on_motorSpeedFast_clicked();

    void on_greenBlinking_clicked();

    void on_yellowBlinking_clicked();

    void on_redBlinking_clicked();

    void on_greenBlinkingOff_clicked();

    void on_yellowBlinkingOff_clicked();

    void on_redBlinkingOff_clicked();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<MonitoringServer> monitoringServer;
    std::unique_ptr<QTcpSocket> controlSock;
    bool monitoringConnected;

    void disableButtons(bool b);
    void connectToMachine(QString ip);
    void startMonitoringServer(QString port);
};

#endif // MAINWINDOW_H
