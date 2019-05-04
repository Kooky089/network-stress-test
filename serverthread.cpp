#include "serverthread.h"

#include <QApplication>


void ServerThread::run() {
    QTcpSocket socket;
    if (!socket.setSocketDescriptor(socketDescriptor)) {
        emit error(socket.error());
        return;
    }
    sendBuffer.resize(bytesPerFrame);
    for (int i = 0; i < sendBuffer.size(); i++) {
        sendBuffer[i] = static_cast<char>(i);
    }

    connect(&socket, &QTcpSocket::disconnected, this, &ServerThread::setStop);

    while(true) {
        QApplication::processEvents();
        if (socket.bytesAvailable() > 0) {
            receiveBuffer = socket.readAll();
        }
        if (socket.bytesToWrite() == 0) {
            socket.write(sendBuffer);
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


