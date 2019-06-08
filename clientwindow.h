#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QWidget>
#include "clientthread.h"

namespace Ui {
class ClientWindow;
}

class ClientWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ClientWindow(QString host, QWidget *parent = nullptr);
    ~ClientWindow();
    QString getLog();

public slots:
    void logInfo(QString);
    void logError(QString);

private slots:
    void on_pushButton_close_clicked();

    void on_pushButton_pause_clicked();

private:
    Ui::ClientWindow *ui;
    QThread *thread;
    ClientThread *clientThread;

signals:
    void pause();
    void resume();
    void stop();

};

#endif // CLIENTWINDOW_H
