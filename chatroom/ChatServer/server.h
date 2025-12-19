#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include "clienthandler.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr) : QTcpServer(parent) {}

protected:
    void incomingConnection(qintptr socketDescriptor) override {
        ClientHandler* handler = new ClientHandler(socketDescriptor);
        connect(handler, &ClientHandler::log, this, &Server::log);
        connect(handler, &ClientHandler::finished, handler, &QObject::deleteLater);
        handler->start();
    }

signals:
    void log(const QString& msg);
};

#endif // SERVER_H
