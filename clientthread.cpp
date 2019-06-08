#include "clientthread.h"

#include <QApplication>
#include <QThread>
#include <QTimer>
#include <QDateTime>

void ClientThread::run() {
    QTcpSocket socket;
    socket.connectToHost(host,25000);
    if (!socket.waitForConnected()) {
        emit setStatus("Could not connect");
        return;
    } else {
        emit setStatus("Connected");
    }
    socket.setReadBufferSize(bytesPerFrame * 16);
    sendBuffer.resize(bytesPerFrame);
    for (int i = 0; i < sendBuffer.size(); i++) {
        sendBuffer[i] = static_cast<char>(i);
    }

    connect(&socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, &ClientThread::error);
    connect(&socket, &QTcpSocket::disconnected, this, &ClientThread::setStop);
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ClientThread::updateStatus);
    timer->start(1000);
 //   socket.setSocketOption(QAbstractSocket::SendBufferSizeSocketOption, QVariant(1024*1024));
    socket.setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, QVariant(1024*1024));

    socket.socketOption(QAbstractSocket::SendBufferSizeSocketOption);
    timestamp = QDateTime::currentDateTime();
    bool isBusy;
    while(true) {
        isBusy = false;
        if (socket.bytesAvailable() >= 0 * bytesPerFrame && !pause) {
            receiveBuffer = socket.read(bytesPerFrame);
            bytesRead += receiveBuffer.size();
            int offset = receiveBuffer[0];
            for (int i = 0; i < receiveBuffer.size(); i++) {
                if (receiveBuffer[i] != static_cast<char>(i+offset)) {
                    errors++;
                }
            }
            isBusy = true;
        }
        QApplication::processEvents();
        if (socket.bytesToWrite() < 10 * bytesPerFrame && !pause) {
            socket.write(sendBuffer);
            bytesWritten += sendBuffer.size();
            isBusy = true;
        }
        if (pause) {
            QThread::msleep(1);
        }
        if (stop) {
            break;
        }
    }
    socket.disconnectFromHost();
    deleteLater();
}

void ClientThread::setStop() {
    stop = true;
}

void ClientThread::setPause() {
    pause = true;
}

void ClientThread::setResume() {
    pause = false;
}

void ClientThread::updateStatus() {
    if (stop) {
        return;
    }
    long long write = (bytesWritten - bytesWrittenLast) / 1000000;
    long long read = (bytesRead - bytesReadLast) / 1000000;

    QString elapsed = QTime(0,0,0).addSecs(timestamp.secsTo(QDateTime::currentDateTime())).toString("hh:mm:ss");
    emit setStatus("Start: " + timestamp.toString("hh:mm:ss dd.MM.yyyy")
               + "\nElapsed: " + elapsed
               + "\nR: " + QString::number(bytesRead/1000000) + " MB (" + QString::number(read) + " MB/s)"
               + "\nS: " + QString::number(bytesWritten/1000000) + " MB (" + QString::number(write) + " MB/s)"
               + "\nErrors: " +  QString::number(errors));
    bytesWrittenLast = bytesWritten;
    bytesReadLast = bytesRead;
}


void ClientThread::error(QAbstractSocket::SocketError socketError)
{
    QString prefix = QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy") + ": ";
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        emit setError(prefix + "Connection closed");
        break;
    case QAbstractSocket::HostNotFoundError:
        emit setError(prefix + "Host not found");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        emit setError(prefix + "Connection refused");
        break;
    default:
        emit setError(prefix + "Connection lost " + QString::number(socketError));
    }
    stop = true;
}

