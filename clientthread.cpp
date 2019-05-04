#include "clientthread.h"

#include <QApplication>

void ClientThread::run() {
    QTcpSocket socket;
    socket.connectToHost(host,25000);
    if (!socket.waitForConnected()) {
        qDebug() << "could not connect";
        return;
    } else {
        emit setStatus("Connected");
    }
    sendBuffer.resize(bytesPerFrame);
    for (int i = 0; i < sendBuffer.size(); i++) {
        sendBuffer[i] = static_cast<char>(i);
    }

    connect(&socket, &QTcpSocket::disconnected, this, &ClientThread::setStop);

    while(true) {
        QApplication::processEvents();
        if (socket.bytesAvailable() > 0) {
            receiveBuffer = socket.readAll();
            count += receiveBuffer.size();
            emit setStatus("Count: " + QString::number(count));
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

void ClientThread::setStop() {
    stop = true;
}
