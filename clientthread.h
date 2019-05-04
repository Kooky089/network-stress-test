#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QObject>
#include <QTcpSocket>

class ClientThread : public QObject
{
    Q_OBJECT
public:
    explicit ClientThread(QString host, QObject *parent = nullptr) : QObject(parent), host(host) {}
signals:
    void finished();
    void setStatus(QString text);

public slots:
    void run();
    void setStop();
    void setPause();
    void setResume();
    void updateStatus();

/*
    void Client::displayError(QAbstractSocket::SocketError socketError)
    {
        switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
            emit logError("Remote host closed connection.");
            break;
        case QAbstractSocket::HostNotFoundError:
            emit logError("Could not connect to host.");
            break;
        case QAbstractSocket::ConnectionRefusedError:
            emit logError("The connection was refused by the peer.");
            break;
        default:
            emit logError("Error.");
        }
        deleteLater();
    }*/

private:
    QByteArray sendBuffer, receiveBuffer;
    int bytesPerFrame = 1024 * 128;
    long long bytesWritten = 0;
    long long bytesRead = 0;
    long long bytesWrittenLast = 0;
    long long bytesReadLast = 0;

    bool stop = false;
    bool pause = false;
    QString host;

};

#endif // CLIENTTHREAD_H
