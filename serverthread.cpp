#include "serverthread.h"

#include <QApplication>
#include <QThread>

void ServerThread::run() {
    QTcpSocket socket;
    if (!socket.setSocketDescriptor(socketDescriptor)) {
        emit error(socket.error());
        return;
    }
    socket.setReadBufferSize(bytesPerFrame * 100);
    sendBuffer.resize(bytesPerFrame);
    for (int i = 0; i < sendBuffer.size(); i++) {
        sendBuffer[i] = static_cast<char>(i);
    }

    connect(&socket, &QTcpSocket::disconnected, this, &ServerThread::setStop);

    bool isBusy;
    while(true) {
        isBusy = false;
        QApplication::processEvents();
        if (socket.bytesAvailable() >= 1 * bytesPerFrame) {
            receiveBuffer = socket.read(bytesPerFrame);
            isBusy = true;
        }
        if (socket.bytesToWrite() < 1 * bytesPerFrame) {
            socket.write(sendBuffer);
            isBusy = true;
        }
        if (!isBusy) {
            //QThread::usleep(1);
        }
        if (stop) {
            break;
        }
    }
    socket.disconnectFromHost();
    deleteLater();
}

void ServerThread::setStop() {
    stop = true;
}


