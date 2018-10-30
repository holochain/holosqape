#include "container.h"
#include <QCryptographicHash>
#include <QFile>
#include <QSettings>
#include <QUrl>
#include <iostream>
#include <QFileInfo>
#include <QStandardPaths>
#include <QDir>
#include <QtShell>
#include "../holochain-rust/dna_c_binding/include/dna_c_binding.h"
#include "../holochain-rust/core_api_c_binding/include/core_api_c_binding.h"
#include "app.h"

Container::Container(QObject *parent) : QObject(parent)
{
    installBuildInRootUIs();
    m_rootUINames = installedRootUIs();
    emit rootUIsChanged();
}

void Container::installBuildInRootUIs() {
    foreach(QFileInfo file_info, QDir(":/rootUIs").entryInfoList()) {
        if(!rootUIsDirectory().exists(file_info.fileName()))
            QtShell::cp("-a", file_info.absoluteFilePath(), rootUIsDirectory().canonicalPath());
    }
}

Dna* loadDnaFile(QString path) {
    if(!path.startsWith(":/"))
        if(QUrl(path).toLocalFile() != "")
            path = QUrl(path).toLocalFile();
    QFile file(path);

    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray contents = file.readAll();
        char* buf = contents.data();

        return holochain_dna_create_from_json(buf);
    } else {
        return 0;
    }
}

void Container::installApp(QString path) {

    Dna *dna = loadDnaFile(path);

    if(!dna) {
        std::cout << "No valid Holochain DNA file" << std::endl;
        std::cout << QString("%1 is not a valid Holochain DNA file!").arg(path).toStdString() << std::endl;
        return;
    } else {
        if(!path.startsWith(":/")) path = QUrl(path).toLocalFile();
        QFile file(path);

        if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QByteArray contents = file.readAll();
            QCryptographicHash hash(QCryptographicHash::Sha256);
            hash.addData(contents);

            QSettings settings;
            settings.beginGroup("dnas");
            settings.setValue(hash.result().toBase64(), contents);
            emit appsChanged();
        }

        holochain_dna_free(dna);
    }
}

QString Container::rootUIsDirectoryPath() {
    return rootUIsDirectory().canonicalPath();
}

QString Container::instancesDirectoryPath() {
    return instancesDirectory().canonicalPath();
}

QString Container::directoryNameForApp(QString app_hash) {
    app_hash.replace("/", "_");
    app_hash.replace("+", "_");
    app_hash.replace("=", "_");
    return app_hash;
}

QDir Container::rootUIsDirectory() {
    QDir app_dir = QDir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    app_dir.mkpath("rootUIs");
    return QDir(app_dir.absoluteFilePath("rootUIs"));
}

QDir Container::instancesDirectory() {
    QDir app_dir = QDir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    app_dir.mkpath("instances");
    return QDir(app_dir.absoluteFilePath("instances"));
}

void Container::installRootUI(QString path) {
    QtShell::cp("-a", QUrl(path).toLocalFile(), rootUIsDirectory().canonicalPath());
    m_rootUINames = installedRootUIs();
    emit rootUIsChanged();
}

QStringList Container::installedApps() {
    QSettings settings;
    settings.beginGroup("dnas");
    return settings.allKeys();
}

QStringList Container::installedRootUIs() {
    QDir rootUIs_directory = rootUIsDirectory();

    QStringList names;
    foreach(QFileInfo file_info, rootUIs_directory.entryInfoList()) {
        if(file_info.isDir() && !file_info.fileName().startsWith("."))
            names << file_info.fileName();
    }

    return names;
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
/*
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
}*/

QList<App*> Container::instances() {
    return m_app_instances;
}

App* Container::instantiate(QString app_hash) {
    App *app;
    QString storage_path = instancesDirectory().absoluteFilePath(directoryNameForApp(app_hash));
    if(instancesDirectory().exists(directoryNameForApp(app_hash))) {
        app = App::load(app_hash, storage_path);
    } else {
        Dna *dna = getDna(app_hash);
        if(!dna) return 0;
        holochain_dna_free(dna);
        instancesDirectory().mkpath(directoryNameForApp(app_hash));
        app = new App(app_hash, storage_path, this);
    }

    m_app_instances.push_back(app);
    return app;
}

App* Container::loadAndInstantiate(QString path) {
    Dna *dna = loadDnaFile(path);
    if(!dna) {
        std::cout << "No valid Holochain DNA file" << std::endl;
        std::cout << QString("%1 is not a valid Holochain DNA file!").arg(path).toStdString() << std::endl;
        return 0;
    } else {
        QString dna_json = QString(holochain_dna_to_json(dna));
        QCryptographicHash hash(QCryptographicHash::Sha256);
        hash.addData(dna_json.toUtf8());
        QString sub_directory = directoryNameForApp(hash.result());
        QString path = QDir(QStandardPaths::writableLocation(QStandardPaths::TempLocation)).absoluteFilePath(sub_directory);
        QtShell::rmdir(path);
        QtShell::mkdir(path);

        return new App(dna, path, this);
    }
}
