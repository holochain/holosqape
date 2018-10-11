#ifndef SOCKETINTERFACE_H
#define SOCKETINTERFACE_H

#include <QObject>
#include <QtWebSockets/QtWebSockets>
#include <QList>
#include <QJsonValue>
#include "container.h"

class SocketInterface : public QObject
{
    Q_OBJECT
public:
    explicit SocketInterface(QObject *parent = nullptr);

signals:

public slots:
    void setContainer(Container* container);

private slots:
    void incomingConnection();
    void message_received(const QString &message);
    void serverError(QWebSocketProtocol::CloseCode closeCode);

private:
    QWebSocketServer m_server;
    QList<QWebSocket*> m_web_sockets;
    Container* m_container;

    QJsonObject executeRPC(QJsonObject rpc);
};

#endif // SOCKETINTERFACE_H
