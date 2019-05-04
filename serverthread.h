#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QObject>
#include <QTcpSocket>


class ServerThread : public QObject
{
    Q_OBJECT
public:
    explicit ServerThread(qintptr socketDescriptor, QObject *parent = nullptr) : QObject(parent), socketDescriptor(socketDescriptor) {}
public slots:
    void run();

signals:
    void finished();
    void error(QTcpSocket::SocketError socketError);
public slots:
    void setStop();


private:
    qintptr socketDescriptor;
    QByteArray sendBuffer, receiveBuffer;
    int bytesPerFrame = 1024*1024;
    bool stop = false;
};

#endif // SERVERTHREAD_H
