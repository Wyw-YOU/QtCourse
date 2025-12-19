#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QThread>
#include <QTcpSocket>
#include <QHostAddress>
#include "chatserver.h"
#include "protocol.h"

class ClientHandler : public QThread
{
    Q_OBJECT
public:
    explicit ClientHandler(qintptr socketDescriptor, QObject *parent = nullptr)
        : QThread(parent), m_socketDescriptor(socketDescriptor) {}

protected:
    void run() override;

signals:
    void log(const QString& msg);

private:
    qintptr m_socketDescriptor;
};

#endif // CLIENTHANDLER_H
