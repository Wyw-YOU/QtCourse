#ifndef MESSAGESERVICE_H
#define MESSAGESERVICE_H

#include <QObject>
#include "Protocol.h"

class MessageService : public QObject
{
    Q_OBJECT
public:
    static MessageService* instance();

signals:

private:
    explicit MessageService(QObject* parent = nullptr);
};

#endif // MESSAGESERVICE_H
