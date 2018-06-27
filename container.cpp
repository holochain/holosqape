#include "container.h"
#include <QCryptographicHash>
#include <QFile>
#include <QSettings>
#include <QUrl>
#include <iostream>
#include "holochain-rust/dna_c_binding/include/dna_c_binding.h"
#include "holochain-rust/core_api_c_binding/include/core_api_c_binding.h"

Container::Container(QObject *parent) : QObject(parent)
{

}

void Container::install_app(QString path) {
    QFile file(QUrl(path).toLocalFile());
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray contents = file.readAll();
        char* buf = contents.data();

        Dna *dna = holochain_dna_create_from_json(buf);

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

        holochain_dna_free(dna);
    }
}

QStringList Container::installedApps() {
    QSettings settings;
    settings.beginGroup("dnas");
    return settings.allKeys();
}

Dna* getDna(QString app_hash) {
    QSettings settings;
    settings.beginGroup("dnas");
    QString dna_string = settings.value(app_hash).toString();
    std::string dna_std_string = dna_string.toStdString();
    const char *buf = dna_std_string.c_str();
    Dna *dna = holochain_dna_create_from_json(buf);
    return dna;
}

QString Container::appName(QString app_hash) {
    Dna *dna = getDna(app_hash);
    char *buf2 = holochain_dna_get_name(dna);
    QString name(buf2);
    holochain_dna_string_free(buf2);
    holochain_dna_free(dna);
    return name;
}

void Container::startApp(QString app_hash) {
    Holochain *hc=0;
    if(m_hc_instances.contains(app_hash))
        hc = m_hc_instances[app_hash];
    else {
        Dna *dna = getDna(app_hash);
        if(dna) {
            hc = holochain_new(dna);
            m_hc_instances[app_hash] = hc;
        }
    }

    if(hc) {
        holochain_start(hc);
    }
}

void Container::stopApp(QString app_hash) {
    if(m_hc_instances.contains(app_hash)) {
        Holochain *hc = m_hc_instances[app_hash];
        holochain_stop(hc);
    }
}

QString Container::call(QString app_hash, QString zome, QString capability, QString function, QString parameters) {
    if(m_hc_instances.contains(app_hash)) {
        Holochain *hc = m_hc_instances[app_hash];
        char *result = holochain_call(hc, zome.toStdString().c_str(), capability.toStdString().c_str(), function.toStdString().c_str(), parameters.toStdString().c_str());
        return QString(result);
    } else {
        return QString("App not running %1").arg(app_hash);
    }
}
