#include "clientthread.h"
#include "clientwindow.h"
#include "ui_clientwindow.h"
#include <QThread>

ClientWindow::ClientWindow(QString host, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientWindow)
{
    ui->setupUi(this);
    ui->label_host->setText(host);
    clientThread = new ClientThread(host);
    thread = new QThread();
    clientThread->moveToThread(thread);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(started()), clientThread, SLOT (run()));
    connect(clientThread, &ClientThread::destroyed, thread, &QThread::quit);
    connect(clientThread, &ClientThread::setStatus, this, &ClientWindow::logInfo);
    connect(clientThread, &ClientThread::setError, this, &ClientWindow::logError);
    connect(this, &ClientWindow::stop, clientThread, &ClientThread::setStop);
    connect(this, &ClientWindow::pause, clientThread, &ClientThread::setPause);
    connect(this, &ClientWindow::resume, clientThread, &ClientThread::setResume);
    thread->start();


    //connect(client, &Client::logError, this, &ClientWindow::logError);


}

ClientWindow::~ClientWindow()
{
    delete ui;
}

void ClientWindow::on_pushButton_close_clicked()
{
    emit stop();
    deleteLater();
}

void ClientWindow::logError(QString error) {
    ui->label_status->setText(ui->label_status->text() + "\n" + error);
}

void ClientWindow::logInfo(QString info) {
    ui->label_status->setText(info);
}

void ClientWindow::on_pushButton_pause_clicked()
{
    if (!ui->pushButton_pause->text().compare("Pause")) {
        emit pause();
        ui->pushButton_pause->setText("Resume");
    } else {
        emit resume();
        ui->pushButton_pause->setText("Pause");
    }

}



