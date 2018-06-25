#include "container.h"
#include <QCryptographicHash>
#include <QFile>
#include <QSettings>
#include <QUrl>

Container::Container(QObject *parent) : QObject(parent)
{

}

void Container::install_app(QString path) {
    QFile file(QUrl(path).toLocalFile());
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray contents = file.readAll();
        QCryptographicHash hash(QCryptographicHash::Sha256);
        hash.addData(contents);

        QSettings settings;
        settings.beginGroup("dnas");
        settings.setValue(hash.result().toBase64(), contents);
        emit appsChanged();
    }
}

QStringList Container::installedApps() {
    QSettings settings;
    settings.beginGroup("dnas");
    return settings.allKeys();
}
