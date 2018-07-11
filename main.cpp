#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "Container.h"
#include "app.h"

int main(int argc, char *argv[])
{
    //qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setOrganizationName("Holochain");
    QCoreApplication::setOrganizationDomain("holochain.org");
    QCoreApplication::setApplicationName("HoloSqape");

    QGuiApplication app(argc, argv);

    Container c;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("Container", &c);
    qmlRegisterType<App>("org.holochain.container", 1, 0, "App");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
