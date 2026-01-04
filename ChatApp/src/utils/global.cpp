// src/utils/global.cpp
#include "global.h"
#include <QJsonDocument>
#include <QCryptographicHash>
#include <QRandomGenerator>

// 实现网络工具函数
namespace Utils {
QByteArray jsonToByteArray(const QJsonObject& json) {
    QJsonDocument doc(json);
    return doc.toJson(QJsonDocument::Compact);
}

QJsonObject byteArrayToJson(const QByteArray& data) {
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        return QJsonObject();
    }
    return doc.object();
}

QString generateMessageId() {
    QString timestamp = QString::number(QDateTime::currentMSecsSinceEpoch(), 16);
    QString random = QString::number(QRandomGenerator::global()->generate(), 16).right(8);
    return timestamp + random;
}

QString generateRequestId() {
    return QString::number(QDateTime::currentMSecsSinceEpoch()) +
           QString::number(QRandomGenerator::global()->generate64() % 10000);
}

QByteArray encodeMessage(NetMessageType type, const QJsonObject& data, quint32 requestId) {
    // 准备消息体
    QByteArray body = jsonToByteArray(data);

    // 准备消息头
    NetMessageHeader header;
    header.messageType = static_cast<quint32>(type);
    header.bodyLength = static_cast<quint32>(body.size());
    header.timestamp = QDateTime::currentMSecsSinceEpoch();
    header.requestId = requestId;

    // 组合消息
    QByteArray message = header.toByteArray();
    message.append(body);

    return message;
}

QPair<NetMessageHeader, QJsonObject> decodeMessage(const QByteArray& data) {
    QPair<NetMessageHeader, QJsonObject> result;

    if (data.size() < static_cast<int>(sizeof(NetMessageHeader))) {
        return result;
    }

    // 解析消息头
    result.first = NetMessageHeader::fromByteArray(data.left(sizeof(NetMessageHeader)));

    // 解析消息体
    if (result.first.bodyLength > 0) {
        QByteArray body = data.mid(sizeof(NetMessageHeader), result.first.bodyLength);
        result.second = byteArrayToJson(body);
    }

    return result;
}
}
