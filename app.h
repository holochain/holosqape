#ifndef APP_H
#define APP_H

#include <QObject>

typedef void Holochain;
typedef void Dna;
class App : public QObject
{
    Q_OBJECT
public:
    App();
    explicit App(QString hash, QObject *parent = nullptr);
    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE QString call(QString zome, QString capability, QString function, QString parameters);
    Q_INVOKABLE QStringList zome_names();
    Q_INVOKABLE QStringList capabilities_for_zome(QString zome);
    Q_INVOKABLE QStringList functions_for_zome_capability(QString zome, QString capability);
    Q_INVOKABLE QStringList parameter_names(QString zome, QString capability, QString function);
    Q_INVOKABLE QString hash() const;
    Q_INVOKABLE QString name() const;

signals:

public slots:

private:
    Holochain* m_instance;
    Dna* m_dna;
    QString m_hash;
};

#endif // APP_H
