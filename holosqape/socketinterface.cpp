#include "socketinterface.h"
#include <iostream>

SocketInterface::SocketInterface(QObject *parent) : QObject(parent),
    m_server(QString("Holochain Container"), QWebSocketServer::NonSecureMode, this)
{
    //("192.168.1.100")
    if (m_server.listen(QHostAddress("192.168.1.100"), 8888)) {
        std::cout << "Listening on port 8888" << std::endl;
    } else {
        std::cout << "Could not listen on port!" << std::endl;
    }
    QObject::connect(&m_server, SIGNAL(newConnection()), this, SLOT(incomingConnection()));
    QObject::connect(&m_server, SIGNAL(serverError(QWebSocketProtocol::CloseCode)), this, SLOT(serverError(QWebSocketProtocol::CloseCode)));

}

void SocketInterface::incomingConnection() {
    std::cout << "Incoming connection.." << std::endl;
    QWebSocket* socket = m_server.nextPendingConnection();
    if(!socket) return;

    std::cout << "Socket good" << std::endl;

    m_web_sockets << socket;
    QObject::connect(socket, &QWebSocket::textMessageReceived,
                     this, &SocketInterface::message_received
                     );
}

void SocketInterface::message_received(const QString &message) {
    std::cout << "Message:" << std::endl;
    std::cout << message.toStdString();
    QWebSocket* socket = qobject_cast<QWebSocket*>(sender());
    socket->sendTextMessage(QString("From container with love: %1").arg(message));
}

void SocketInterface::serverError(QWebSocketProtocol::CloseCode closeCode) {
    std::cout << "Error:" << closeCode << std::endl;
}
