#include <QtCore>
#include "console.h"

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
    parser.addPositionalArgument("script", QCoreApplication::translate("main", "JS file to run"));
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

    const QStringList positionalArguments = parser.positionalArguments();
    if (positionalArguments.isEmpty() && !interactive) {
        std::cout << "Argument 'script' missing." << std::endl;
        return 1;
    }
    if (positionalArguments.size() > 1) {
        std::cout << "Several 'script' arguments specified." << std::endl;
        return 1;
    }

    Console t;
    t.setInteractive(parser.isSet(interactiveOption));

    if (positionalArguments.size() == 1) {
        QString scriptPath = positionalArguments.first();
        t.setScriptPath(scriptPath);
    }

    QTimer::singleShot(1, &t, SLOT(run()));

    return app.exec();
}
