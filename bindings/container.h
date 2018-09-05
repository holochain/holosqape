#ifndef CONTAINER_H
#define CONTAINER_H

#include <QObject>
#include <QHash>
#include <QQmlApplicationEngine>
#include <QDir>
#include <QStringListModel>

typedef void Holochain;
typedef void Dna;
class App;
class Container : public QObject
{
    Q_OBJECT
        Q_PROPERTY(QStringList rootUINames MEMBER m_rootUINames NOTIFY rootUIsChanged)
public:
    explicit Container(QObject *parent = nullptr);
    Q_INVOKABLE QStringList installedApps();
    Q_INVOKABLE QStringList installedRootUIs();
    Q_INVOKABLE QString appName(QString app_hash);
    Q_INVOKABLE QList<App*> instances();
    Q_INVOKABLE App* instantiate(QString app_hash);
    Q_INVOKABLE App* loadAndInstantiate(QString path);
    Q_INVOKABLE QString rootUIsDirectoryPath();
    QDir rootUIsDirectory();


signals:
    void appsChanged();
    void rootUIsChanged();

public slots:
    void installApp(QString path);
    void installRootUI(QString path);
    void installBuildInRootUIs();

private:
    QList<App*> m_app_instances;
    QStringList m_rootUINames;

};

Dna* getDna(QString app_hash);

#endif // CONTAINER_H
