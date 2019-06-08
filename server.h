#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>

#include <QList>
class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
protected:
    void incomingConnection(qintptr socketDescriptor) override;
};

#endif // SERVER_H
