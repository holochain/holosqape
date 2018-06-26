#ifndef CONTAINER_H
#define CONTAINER_H

#include <QObject>
#include <QHash>

typedef void Holochain;
class Container : public QObject
{
    Q_OBJECT
public:
    explicit Container(QObject *parent = nullptr);
    Q_INVOKABLE QStringList installedApps();
    Q_INVOKABLE QString appName(QString app_hash);
    Q_INVOKABLE void startApp(QString app_hash);
    Q_INVOKABLE void stopApp(QString app_hash);
    Q_INVOKABLE QString call(QString app_hash, QString zome, QString capability, QString function, QString parameters);

signals:
    void appsChanged();

public slots:
    void install_app(QString path);

private:
    QHash<QString, Holochain*> m_hc_instances;

};

#endif // CONTAINER_H
