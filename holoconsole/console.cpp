#include "console.h"
#include <QTimer>
#include <QJSEngine>

Console::Console(QObject *parent) : QObject(parent)
{
    QJSValue container = m_engine.newQObject(&m_container);
    QJSValue console = m_engine.newQObject(this);
    m_engine.globalObject().setProperty("Container", container);
    m_engine.globalObject().setProperty("console", console);

    qRegisterMetaType<App*>();
    qRegisterMetaType<QTimer*>();

    run_script_file(":/shims.js");
}

QTimer* Console::setTimeout(QJSValue fn, int milliseconds)
{
  if (fn.isCallable())
  {
    QTimer *timer = new QTimer(this);
    m_callbacks[timer] = fn;
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer->setSingleShot(true);
    timer->start(milliseconds);
    return timer;
  }

  return 0;
}

void Console::clearTimout(QTimer *timeout) {
    timeout->stop();
    m_callbacks.remove(timeout);
    timeout->deleteLater();
}

QTimer* Console::getTimer() {
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), timer, SLOT(deleteLater()));
    timer->setSingleShot(true);
    return timer;
}

void Console::log(QString message) {
    std::cout << message.toStdString() << std::endl;
}

void Console::timeout() {
    QTimer *timer = qobject_cast<QTimer*>(sender());
    if(!timer) {
        std::cout << "Timout from non-timer?!" << std::endl;
        return;
    }

    m_callbacks[timer].call();
    m_callbacks.remove(timer);
    timer->deleteLater();

    if(m_callbacks.isEmpty() && !m_interactive)
        QCoreApplication::quit();

}

void Console::setInteractive(bool is_interactive) {
    m_interactive = is_interactive;
}

void Console::setScriptPath(QString script_path) {
    m_script_path = script_path;
}

QJSValue Console::run_script_file(QString scriptPath) {
    QFile scriptFile(scriptPath);
    if (!scriptFile.open(QIODevice::ReadOnly)) {
        std::cout << "Could not open file " << scriptPath.toStdString() << "!" << std::endl;
    } else {
        QTextStream stream(&scriptFile);
        QString contents = stream.readAll();
        scriptFile.close();
        QJSValue result = m_engine.evaluate(contents, m_script_path);
        if(result.isError()) {
            qDebug()
                        << "Uncaught exception at line"
                        << result.property("lineNumber").toInt()
                        << ":" << result.toString() << "\n"
                        << result.property("message").toString() << "\n"
                        << result.property("stack").toString() << "\n";
        }

        return result;
    }
}

void Console::run() {

    if (m_script_path != "") {
        QJSValue result = run_script_file(m_script_path);
        std::cout << "=>" << result.toString().toStdString() << std::endl;
        m_script_path = "";
    }

    if(m_interactive) {
        std::cout << "> ";
        std::string input;
        std::getline(std::cin, input);
        if(input == "exit")
            QCoreApplication::quit();
        else {
            QJSValue result = m_engine.evaluate(QString(input.c_str()));
            std::cout << result.toString().toStdString() << std::endl;
        }
    }

    QTimer::singleShot(1, this, SLOT(run()));
}
