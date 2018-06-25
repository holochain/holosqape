#ifndef CONTAINER_H
#define CONTAINER_H

#include <QObject>

class Container : public QObject
{
    Q_OBJECT
public:
    explicit Container(QObject *parent = nullptr);
    Q_INVOKABLE QStringList installedApps();

signals:
    void appsChanged();

public slots:
    void install_app(QString path);

};

#endif // CONTAINER_H
