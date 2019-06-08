#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clientwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QFileDialog>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server.listen(QHostAddress::Any, 25000);
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    ui->label_server->setText("My IPs:");
    for (auto address = ipAddressesList.begin(); address != ipAddressesList.end(); address++) {
        if (!address->isLoopback() && address->protocol() == QAbstractSocket::IPv4Protocol) {
            ui->label_server->setText(ui->label_server->text() + "\n " + address->toString());
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_connect_clicked()
{
    QString host = ui->lineEdit_host->text();
    if (!host.isEmpty()) {
        ClientWindow *test = new ClientWindow(host, this);
        ui->verticalLayout_box->layout()->addWidget(test);
    } else {
        QMessageBox::information(this, tr(""),tr("Host name must not be empty."));
    }
}

void MainWindow::connectToHostViaCMD(QString host) {
    ClientWindow *test = new ClientWindow(host, this);
    ui->verticalLayout_box->layout()->addWidget(test);
}

void MainWindow::startTimer(QString path, int time) {
    if (time <= 0) {
        return;
    }
    logfilePath = path;
    QFile file( logfilePath );
    if (!file.open(QIODevice::ReadWrite) ) {
        return;
    }
    ui->label_log->setText("Will write logfile. Path: '" + path + "' Time: " +
                              QDateTime::currentDateTime().addSecs(time).toString("hh:mm:ss dd.MM.yyyy"));
    QTimer::singleShot(time*1000 , this, [this,path](){
        QString msg;
        if (this->saveLog(logfilePath)) {
            msg = "Logfile successfully written to " + logfilePath;
        } else {
            msg = "Failed to write logfile to " + logfilePath;
        }

        this->ui->label_log->setText(msg);
    });
}

void MainWindow::on_lineEdit_host_returnPressed()
{
    on_pushButton_connect_clicked();
}

bool MainWindow::saveLog(QString filename)
{
    qDebug() << "log " << filename;
    QFile file( filename );
    if ( file.open(QIODevice::ReadWrite) ) {
        QTextStream stream( &file );
        QList<ClientWindow *> childrenList = this->findChildren<ClientWindow *>();
        for (auto &element : childrenList) {
            stream << element->getLog() + "\n\n";
        }
    } else {
        return false;
    }
    return true;
}

void MainWindow::on_pushButton_log_clicked()
{
    saveLog(QFileDialog::getSaveFileName(this,tr("Save logfile"), logfilePath, tr("Log Files (*.log)")));
}

