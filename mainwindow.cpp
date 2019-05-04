#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clientwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QNetworkInterface>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server.listen(QHostAddress::Any, 25000);
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    qDebug() << ipAddressesList << "\n";

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
        QMessageBox::information(this, tr(""),
                                 tr("Host name must not be empty."));
    }
}

void MainWindow::on_lineEdit_host_returnPressed()
{
    on_pushButton_connect_clicked();
}
