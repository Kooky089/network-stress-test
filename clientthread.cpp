#include "clientthread.h"

#include <QApplication>
#include <QThread>
#include <QTimer>

void ClientThread::run() {
    QTcpSocket socket;
    socket.connectToHost(host,25000);
    if (!socket.waitForConnected()) {
        emit setStatus("Could not connect.");
        return;
    } else {
        emit setStatus("Connected.");
    }
    socket.setReadBufferSize(bytesPerFrame * 100);
    sendBuffer.resize(bytesPerFrame);
    for (int i = 0; i < sendBuffer.size(); i++) {
        sendBuffer[i] = static_cast<char>(i);
    }

    connect(&socket, &QTcpSocket::disconnected, this, &ClientThread::setStop);
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ClientThread::updateStatus);
    timer->start(1000);

    bool isBusy;
    while(true) {
        isBusy = false;
        QApplication::processEvents();
        if (socket.bytesAvailable() >= 1 * bytesPerFrame && !pause) {
            receiveBuffer = socket.read(bytesPerFrame);
            bytesRead += receiveBuffer.size();
            isBusy = true;
        }
        if (socket.bytesToWrite() < 1 * bytesPerFrame && !pause) {
            socket.write(sendBuffer);
            bytesWritten += sendBuffer.size();
            isBusy = true;
        }
        if (pause) {
            QThread::usleep(1);
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
    int write = (bytesWritten - bytesWrittenLast) / 1000000;
    int read = (bytesRead - bytesReadLast) / 1000000;
    emit setStatus("Read/Write: " + QString::number(read) + " " + QString::number(write));
    bytesWrittenLast = bytesWritten;
    bytesReadLast = bytesRead;
}

