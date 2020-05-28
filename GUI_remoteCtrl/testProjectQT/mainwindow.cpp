#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "constants.h"

#include <QTextStream>
#include <QMessageBox>
#include <QHostAddress>
 #include <QDebug>

#include <iostream>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    disableButtons(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectToMachine(QString ip)
{
    controlSock = std::make_unique<QTcpSocket>();
    controlSock->connectToHost(ip, CONTROL_PORT);
    if (controlSock->waitForConnected(1000)) {
        qDebug() << "Connected to band: " << ip.toLatin1();
        disableButtons(false);
        ui->controllerConnectedStatus->setText("Connected");
    }
    else {
        qDebug() << "Timeout while connecting to " << ip.toLatin1();
    }
}

void MainWindow::disableButtons(bool b)
{
    ui->greenOn->setDisabled(b);
    ui->greenOff->setDisabled(b);
    ui->yellowOn->setDisabled(b);
    ui->yellowOff->setDisabled(b);
    ui->redOn->setDisabled(b);
    ui->redOff->setDisabled(b);
    ui->stopMotor->setDisabled(b);
    ui->motorRight->setDisabled(b);
    ui->motorLeft->setDisabled(b);
    ui->openGate->setDisabled(b);
    ui->closeGate->setDisabled(b);
    ui->greenBlinking->setDisabled(b);
    ui->yellowBlinking->setDisabled(b);
    ui->redBlinking->setDisabled(b);
    ui->greenBlinkingOff->setDisabled(b);
    ui->yellowBlinkingOff->setDisabled(b);
    ui->redBlinkingOff->setDisabled(b);
    ui->motorSpeedFast->setDisabled(b);
    ui->motorSpeedSlow->setDisabled(b);
}

void MainWindow::on_stopMotor_clicked()
{
    qDebug() << "Motor stopped...";
    controlSock->write(MOTOR_STOP);
}

void MainWindow::on_openGate_clicked()
{
    qDebug() << "Opening the gate...";
    controlSock->write(GATE_OPEN);
}

void MainWindow::on_closeGate_clicked()
{
    qDebug() << "Closing the gate...";
    controlSock->write(GATE_CLOSE);
}

void MainWindow::on_greenOn_clicked()
{
    qDebug() << "Turning greenlamp on...";
    controlSock->write(GREEN_LIGHT_ON);
}

void MainWindow::on_greenOff_clicked()
{
    qDebug() << "Turning greenlamp off...";
    controlSock->write(GREEN_LIGHT_OFF);
}

void MainWindow::on_yellowOn_clicked()
{
    qDebug() << "Turning yellowlamp on...";
    controlSock->write(YELLOW_LIGHT_ON);
}

void MainWindow::on_yellowOff_clicked()
{
    qDebug() << "Turning yellowlamp off...";
    controlSock->write(YELLOW_LIGHT_OFF);
}

void MainWindow::on_redOn_clicked()
{
    qDebug() << "Turning redlamp on...";
    controlSock->write(RED_LIGHT_ON);
}

void MainWindow::on_redOff_clicked()
{
    qDebug() << "Turning redlamp off...";
    controlSock->write(RED_LIGHT_OFF);
}

void MainWindow::on_motorRight_clicked()
{
    qDebug() << "Motor move right";
    controlSock->write(MOTOR_RIGHT);
    controlSock->write(MOTOR_ON);
}

void MainWindow::on_motorLeft_clicked()
{
    qDebug() << "Motor move left";
    controlSock->write(MOTOR_LEFT);
    controlSock->write(MOTOR_ON);
}

void MainWindow::on_controlIP_returnPressed()
{
    qDebug() << "Connecting to band: " << ui->controlIP->text().toLatin1();
    connectToMachine(ui->controlIP->text());
}

void MainWindow::on_monitoringPort_returnPressed()
{
    QString port = ui->monitoringPort->text();
    qDebug() << "Starting monitoring server at port" << port.toLatin1();

    monitoringServer = std::make_unique<MonitoringServer>(ui->monitorLog, ui->monitoringStatus, this);
    /* listen for connections to monitoring server */
    if (!monitoringServer->listen(QHostAddress::Any, port.toInt())) {
        QMessageBox::critical(this, tr("Threaded Monitoring Server"),
                              tr("Unable to start the server: %1.")
                              .arg(monitoringServer->errorString()));
    }
}

void MainWindow::on_motorSpeedSlow_clicked()
{
    qDebug() << "Motor move slow";
    controlSock->write(MOTOR_SLOW);
}

void MainWindow::on_motorSpeedFast_clicked()
{
    qDebug() << "Motor move fast";
    controlSock->write(MOTOR_FAST);
}

void MainWindow::on_greenBlinking_clicked()
{
    qDebug() << "Greenblinking activated...";
    controlSock->write(GREEN_LIGHT_BLINK_FAST);
}

void MainWindow::on_yellowBlinking_clicked()
{
    qDebug() << "Yellowblinking activated...";
    controlSock->write(YELLOW_LIGHT_BLINK_FAST);
}

void MainWindow::on_redBlinking_clicked()
{
    qDebug() << "Redblinking activated...";
    controlSock->write(RED_LIGHT_BLINK_FAST);
}

void MainWindow::on_greenBlinkingOff_clicked()
{
    qDebug() << "Greenblinking deactivated...";
    controlSock->write(GREEN_LIGHT_BLINK_OFF);
}

void MainWindow::on_yellowBlinkingOff_clicked()
{
    qDebug() << "Yellowblinking deactivated...";
    controlSock->write(YELLOW_LIGHT_BLINK_OFF);
}

void MainWindow::on_redBlinkingOff_clicked()
{
    qDebug() << "Redblinking deactivated...";
    controlSock->write(RED_LIGHT_BLINK_OFF);
}
