#include "socketinterface.h"
#include <iostream>
#include <QJsonDocument>
#include "app.h"

SocketInterface::SocketInterface(QObject *parent) : QObject(parent),
    m_server(QString("Holochain Container"), QWebSocketServer::NonSecureMode, this),
    m_container(0)
{
    //("192.168.1.100")
    if (m_server.listen(QHostAddress(QHostAddress::LocalHost), 8888)) {
        std::cout << "Listening on port 8888" << std::endl;
    } else {
        std::cout << "Could not listen on port!" << std::endl;
    }
    QObject::connect(&m_server, SIGNAL(newConnection()), this, SLOT(incomingConnection()));
    QObject::connect(&m_server, SIGNAL(serverError(QWebSocketProtocol::CloseCode)), this, SLOT(serverError(QWebSocketProtocol::CloseCode)));

}

void SocketInterface::setContainer(Container* container) {
    m_container = container;
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
    std::cout << "Message: ";
    std::cout << message.toStdString() << std::endl;

    QWebSocket* socket = qobject_cast<QWebSocket*>(sender());


    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());

    if(!doc.isObject()) {
        std::cout << "not an object" << std::endl;
        socket->sendTextMessage(QString('{"error": "receveid non-JSON-object"}'));
        return;
    }

    QJsonObject rpc = doc.object();

    if(!rpc.contains("id")) {
        std::cout << "no id given" << std::endl;
        socket->sendTextMessage(QString("{\"error\": \"missing id\"}"));
        return;
    }

    if(!rpc.contains("method")) {
        std::cout << "no method given" << std::endl;
        socket->sendTextMessage(QString("{\"error\": \"missing method\", \"id\": %1}").arg(rpc.value("id").toInt()));
        return;
    }

    if(!rpc.contains("params")) {
        std::cout << "no params given" << std::endl;
        socket->sendTextMessage(QString("{\"error\": \"missing params\", \"id\": %1}").arg(rpc.value("id").toInt()));
        return;
    }



    std::cout << "got method and params" << std::endl;


    QStringList method = rpc.value("method").toString().split("/");
    QString dna = method[0];
    QString zome = method[1];
    QString cap = method[2];
    QString function = method[3];
    QString params = QJsonDocument(rpc.value("params").toObject()).toJson();

    std::cout << "RPC good: " << std::endl;
    std::cout << dna.toStdString() << " " << zome.toStdString() << " " << cap.toStdString() << " " << function.toStdString() << std::endl;
    std::cout << params.toStdString() << std::endl;


    App* app = m_container->instantiate(dna);

    if(!app) {
        std::cout << "app == 0" << std::endl;
        socket->sendTextMessage(QString("{\"error\": \"could not instantiate app\", \"id\": %1}").arg(rpc.value("id").toInt()));
        return;
    }

    app->start();

    QString result = app->call(zome, cap, function, params);

    socket->sendTextMessage(QString("{\"result\": %1, \"id\": %2}").arg(result).arg(rpc.value("id").toInt()));

    std::cout << "Result: " << result.toStdString() << std::endl;

}

void SocketInterface::serverError(QWebSocketProtocol::CloseCode closeCode) {
    std::cout << "Error:" << closeCode << std::endl;
}
