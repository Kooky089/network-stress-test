#include "serverthread.h"

#include <QApplication>
#include <QThread>

void ServerThread::run() {
    QTcpSocket socket;
    if (!socket.setSocketDescriptor(socketDescriptor)) {
        emit error(socket.error());
        return;
    }
    socket.setReadBufferSize(bytesPerFrame * 16);
    sendBuffer.resize(bytesPerFrame);
    for (int i = 0; i < sendBuffer.size(); i++) {
        sendBuffer[i] = static_cast<char>(i);
    }

    connect(&socket, &QTcpSocket::disconnected, this, &ServerThread::setStop);

    socket.setSocketOption(QAbstractSocket::SendBufferSizeSocketOption, QVariant(1024*1024));
    socket.setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, QVariant(1024*1024));


    bool isBusy;
    while(true) {
        isBusy = false;
        QApplication::processEvents();
        if (socket.bytesAvailable() > 0 * bytesPerFrame) {
            receiveBuffer = socket.read(bytesPerFrame);
            isBusy = true;
        }
        if (socket.bytesToWrite() < 10 * bytesPerFrame) {
            socket.write(sendBuffer);
            isBusy = true;
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


