#include <QtCore>
#include "console.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setOrganizationName("Holochain");
    QCoreApplication::setOrganizationDomain("holochain.org");
    QCoreApplication::setApplicationName("hcshell");
    QCoreApplication::setApplicationVersion("0.0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("A GUI-less Holochain app container - with interactive JS shell");
    const QCommandLineOption helpOption = parser.addHelpOption();
    const QCommandLineOption versionOption = parser.addVersionOption();
    parser.addPositionalArgument("script", QCoreApplication::translate("main", "JS file to run"));

    QCommandLineOption interactiveOption(QStringList() << "i" << "interactive", QCoreApplication::translate("main", "Open interactive shell"));
    parser.addOption(interactiveOption);

    QCommandLineOption websocketOption(QStringList() << "w" << "websocket-interface", QCoreApplication::translate("main", "Open WebSocket interface for web based UIs on given port"), "port");
    parser.addOption(websocketOption);

    // Process the actual command line arguments given by the user
    parser.process(app);

    const QStringList args = parser.positionalArguments();

    bool interactive = parser.isSet(interactiveOption);

    if(parser.isSet(helpOption)){
        parser.showHelp();
    }

    if(parser.isSet(versionOption)) {
        parser.showVersion();
    }

    const QStringList positionalArguments = parser.positionalArguments();
    if (positionalArguments.isEmpty() && !parser.isSet(versionOption) && !parser.isSet(helpOption)) {
        interactive = true;
    }
    if (positionalArguments.size() > 1) {
        std::cout << "Several 'script' arguments specified." << std::endl;
        return 1;
    }

    Console *console = new Console;
    console->setInteractive(interactive);
    if(interactive)
        console->printHeader();

    if (positionalArguments.size() == 1) {
        QString scriptPath = positionalArguments.first();
        console->setScriptPath(scriptPath);
    }

    QTimer::singleShot(1, console, SLOT(run()));

    if (parser.isSet(websocketOption)) {
        uint port = parser.value(websocketOption).toUInt();
        console->startWebSocketServer(port);
    }

    return app.exec();
}
