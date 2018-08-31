#ifndef CONTAINER_H
#define CONTAINER_H

#include <QObject>
#include <QHash>

typedef void Holochain;
typedef void Dna;
class App;
class Container : public QObject
{
    Q_OBJECT
public:
    explicit Container(QObject *parent = nullptr);
    Q_INVOKABLE QStringList installedApps();
    Q_INVOKABLE QString appName(QString app_hash);
    Q_INVOKABLE QList<App*> instances();
    Q_INVOKABLE App* instantiate(QString app_hash);
    Q_INVOKABLE App* loadAndInstantiate(QString path);

signals:
    void appsChanged();

public slots:
    void installApp(QString path);

private:
    QList<App*> m_app_instances;

};

Dna* getDna(QString app_hash);

#endif // CONTAINER_H
