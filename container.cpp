#include "container.h"
#include <QCryptographicHash>
#include <QFile>
#include <QSettings>
#include <QUrl>
#include <iostream>
#include "holochain-rust/hc_dna_c_binding/include/hc_dna_c_binding.h"

Container::Container(QObject *parent) : QObject(parent)
{

}

void Container::install_app(QString path) {
    QFile file(QUrl(path).toLocalFile());
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray contents = file.readAll();
        char* buf = contents.data();

        Dna *dna = hc_dna_create_from_json(buf);

        if(!dna) {
            std::cout << "No valid Holochain DNA file" << std::endl;
            std::cout << QString("%1 is not a valid Holochain DNA file!").arg(path).toStdString() << std::endl;
            return;
        }

        QCryptographicHash hash(QCryptographicHash::Sha256);
        hash.addData(contents);

        QSettings settings;
        settings.beginGroup("dnas");
        settings.setValue(hash.result().toBase64(), contents);
        emit appsChanged();

        hc_dna_free(dna);
    }
}

QStringList Container::installedApps() {
    QSettings settings;
    settings.beginGroup("dnas");
    return settings.allKeys();
}
