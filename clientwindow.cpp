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
    connect(this, &ClientWindow::stop, clientThread, &ClientThread::setStop);
    //connect(this, &ClientWindow::pause, client, &Client::pauseThread);
    //connect(this, &ClientWindow::resume, client, &Client::resumeThread);
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
    ui->label_status->setText(error);
}

void ClientWindow::logInfo(QString info) {
    ui->label_status->setText(info);
}
