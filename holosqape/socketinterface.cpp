#include "socketinterface.h"
#include <iostream>

SocketInterface::SocketInterface(QObject *parent) : QObject(parent),
    m_server(QString("Holochain Container"), QWebSocketServer::SecureMode, this)
{
    m_server.listen(QHostAddress::Any, 8888);
}

void SocketInterface::incomingConnection() {
    QWebSocket* socket = m_server.nextPendingConnection();
    if(!socket) return;

    m_web_sockets << socket;
    QObject::connect(socket, &QWebSocket::textMessageReceived,
                     this, &SocketInterface::message_received
                     );
}

void SocketInterface::message_received(const QString &message) {
    std::cout << message.toStdString();
    QWebSocket* socket = qobject_cast<QWebSocket*>(sender());
    socket->sendTextMessage(QString("From container with love: %1").arg(message));
}
