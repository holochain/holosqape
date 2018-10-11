#include "socketinterface.h"
#include <QJsonDocument>
#include "app.h"

SocketInterface::SocketInterface(QObject *parent) : QObject(parent),
    m_server(QString("Holochain Container"), QWebSocketServer::NonSecureMode, this),
    m_container(0)
{
    if (m_server.listen(QHostAddress(QHostAddress::LocalHost), 8888)) {
        qDebug() << "Listening on ws://localhost:8888";
    } else {
        qDebug() << "Could not listen on port 8888!";
    }
    QObject::connect(&m_server, SIGNAL(newConnection()), this, SLOT(incomingConnection()));
    QObject::connect(&m_server, SIGNAL(serverError(QWebSocketProtocol::CloseCode)), this, SLOT(serverError(QWebSocketProtocol::CloseCode)));

}

void SocketInterface::setContainer(Container* container) {
    m_container = container;
}


void SocketInterface::incomingConnection() {
    QWebSocket* socket = m_server.nextPendingConnection();
    if(!socket) return;

    qDebug() << "New websocket connection";

    m_web_sockets << socket;
    QObject::connect(socket, &QWebSocket::textMessageReceived,
                     this, &SocketInterface::message_received
                     );
}

void SocketInterface::message_received(const QString &message) {
    QWebSocket* socket = qobject_cast<QWebSocket*>(sender());


    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());

    if(!doc.isObject()) {
        qDebug() << "Received RPC that is not an object:";
        qDebug() << message;
        socket->sendTextMessage(QString('{"error": "receveid non-JSON-object"}'));
        return;
    }

    QJsonObject rpc = doc.object();

    if(!rpc.contains("id")) {
        qDebug() << "Received RPC without id:";
        qDebug() << message;
        socket->sendTextMessage(QString("{\"error\": \"missing id\"}"));
        return;
    }

    if(!rpc.contains("method")) {
        qDebug() << "Received RPC without method:";
        qDebug() << message;
        socket->sendTextMessage(QString("{\"error\": \"missing method\", \"id\": %1}").arg(rpc.value("id").toInt()));
        return;
    }

    if(!rpc.contains("params")) {
        qDebug() << "Received RPC without params:";
        qDebug() << message;
        socket->sendTextMessage(QString("{\"error\": \"missing params\", \"id\": %1}").arg(rpc.value("id").toInt()));
        return;
    }

    qDebug() << "RPC Request: " << message;

    QStringList method = rpc.value("method").toString().split("/");
    QString dna = method[0];
    QString zome = method[1];
    QString cap = method[2];
    QString function = method[3];
    QString params = QJsonDocument(rpc.value("params").toObject()).toJson();


    App* app = m_container->instantiate(dna);

    if(!app) {
        qDebug() << "app == 0";
        socket->sendTextMessage(QString("{\"error\": \"could not instantiate app\", \"id\": %1}").arg(rpc.value("id").toInt()));
        return;
    }

    app->start();

    QString result = app->call(zome, cap, function, params);

    socket->sendTextMessage(QString("{\"result\": %1, \"id\": %2}").arg(result).arg(rpc.value("id").toInt()));

    qDebug() << "RPC Result: " << result;

}

void SocketInterface::serverError(QWebSocketProtocol::CloseCode closeCode) {
    qDebug() << "WebSocketServer Error: " << closeCode;
}


