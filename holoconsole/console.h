#ifndef SCRIPTTIMER_H
#define SCRIPTTIMER_H

#include <QObject>
#include <QJSValue>
#include <QTimer>
#include "container.h"
#include "app.h"
#include <QQmlContext>
#include <QQmlExpression>
#include <QtConcurrent>
#include <iostream>
#include <string>
#include <QMetaObject>
#include <QQmlApplicationEngine>
#include <QHash>

class Console : public QObject
{
    Q_OBJECT
public:
    explicit Console(QObject *parent = nullptr);
    Q_INVOKABLE QTimer* setTimeout(QJSValue fn, int milliseconds);
    Q_INVOKABLE QTimer* getTimer();
    Q_INVOKABLE void clearTimout(QTimer *timeout);
    Q_INVOKABLE void log(QString message);


signals:

public slots:

    void timeout();
    void setInteractive(bool is_interactive);
    void setScriptPath(QString script_path);
    void run();

private:
    void run_script_file(QString scriptPath);

    QJSEngine m_engine;
    Container m_container;
    bool m_interactive;
    QString m_script_path;
    QHash<QTimer*, QJSValue> m_callbacks;
};

#endif // SCRIPTTIMER_H
