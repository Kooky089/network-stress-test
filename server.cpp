#include "server.h"
#include "serverthread.h"
#include <QNetworkInterface>
#include <QString>
#include <QThread>

Server::Server(QObject *parent) : QTcpServer(parent) {}

void Server::incomingConnection(qintptr socketDescriptor)
{
    QThread* thread = new QThread;
    ServerThread *serverThread = new ServerThread(socketDescriptor);
    serverThread->moveToThread(thread);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL (started()), serverThread, SLOT (run()));
    connect(serverThread, &ServerThread::destroyed, thread, &QThread::quit);
    thread->start();
}
