#include <QtCore>
#include "container.h"
#include "app.h"
#include <QJSEngine>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setOrganizationName("Holochain");
    QCoreApplication::setOrganizationDomain("holochain.org");
    QCoreApplication::setApplicationName("HoloSqape");
    QCoreApplication::setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("A GUI-less Holochain app container - with interactive JS shell");
    const QCommandLineOption helpOption = parser.addHelpOption();
    const QCommandLineOption versionOption = parser.addVersionOption();
    //parser.addPositionalArgument("source", QCoreApplication::translate("main", "Source file to copy."));
    //parser.addPositionalArgument("destination", QCoreApplication::translate("main", "Destination directory."));

    QCommandLineOption interactiveOption(QStringList() << "i" << "interactive", QCoreApplication::translate("main", "Open interactive shell"));
    parser.addOption(interactiveOption);


    // An option with a value
    QCommandLineOption targetDirectoryOption(QStringList() << "t" << "target-directory",
            QCoreApplication::translate("main", "Copy all source files into <directory>."),
            QCoreApplication::translate("main", "directory"));
    parser.addOption(targetDirectoryOption);

    // Process the actual command line arguments given by the user
    parser.process(app);

    const QStringList args = parser.positionalArguments();
    // source is args.at(0), destination is args.at(1)

    bool interactive = parser.isSet(interactiveOption);

    if(parser.isSet(helpOption) ){
        parser.showHelp();
    }

    if(parser.isSet(versionOption)) {
        parser.showVersion();
    }

    if(interactive) {
        Container c;

        QJSEngine engine;
        QJSValue container = engine.newQObject(&c);
        engine.globalObject().setProperty("Container", container);
        //qmlRegisterType<App>("org.holochain.container", 1, 0, "App");
        qRegisterMetaType<App*>();

        bool exit = false;
        while(!exit) {
            std::cout << "> ";
            std::string input;
            std::getline(std::cin, input);
            if(input == "exit") return 0;

            QJSValue result = engine.evaluate(QString(input.c_str()));
            std::cout << result.toString().toStdString() << std::endl;
        }
    }
}
