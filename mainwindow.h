#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "server.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void connectToHostViaCMD(QString host);
    void startTimer(QString logfile, int time);

private slots:
    void on_pushButton_connect_clicked();
    void on_lineEdit_host_returnPressed();
    void on_pushButton_log_clicked();
    bool saveLog(QString filename);
private:
    Ui::MainWindow *ui;
    Server server;
    QString logfilePath{};


};

#endif // MAINWINDOW_H
