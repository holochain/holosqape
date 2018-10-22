#include "app.h"
#include "../holochain-rust/dna_c_binding/include/dna_c_binding.h"
#include "../holochain-rust/core_api_c_binding/include/core_api_c_binding.h"
#include "container.h"

App::App() : QObject(0) {
    m_dna = 0;
    m_instance = 0;
}

App::App(QString hash, QString storage_path, QObject *parent) : QObject(parent), m_hash(hash)
{
    m_dna = getDna(m_hash);
    if(m_dna){
        m_instance = holochain_new(m_dna, storage_path.toStdString().c_str());
        // need to recreate a dna object because holochain_new() consumes it..
        // Might wanna change that on the rust side?
        m_dna = getDna(m_hash);
    } else {
        m_instance = 0;
    }
}


App::App(Dna *dna, QString storage_path, QObject *parent) : QObject(parent)
{
    m_dna = dna;
    if(m_dna){
        // need to clone a dna object because holochain_new() consumes it..
        // Might wanna change that on the rust side?
        Dna *dna_clone = holochain_dna_create_from_json(holochain_dna_to_json(m_dna));
        m_instance = holochain_new(dna_clone, storage_path.toStdString().c_str());
    } else {
        m_instance = 0;
    }
}

void App::start(){
    if(m_instance) {
        holochain_start(m_instance);
    }
}

void App::stop(){
    if(m_instance) {
        holochain_stop(m_instance);
    }
}

QString App::call(QString zome, QString capability, QString function, QString parameters){
    if(m_instance) {
        char *result = holochain_call(m_instance, zome.toStdString().c_str(), capability.toStdString().c_str(), function.toStdString().c_str(), parameters.toStdString().c_str());
        return QString(result);
    }
    return QString();
}

QStringList App::zome_names() {
    if(!m_dna) return QStringList();

    CStringVec names;
    holochain_dna_get_zome_names(m_dna, &names);
    QStringList result;
    for(unsigned int i = 0; i < names.len; i++)
        result << QString("%1").arg(names.ptr[i]);
    return result;
}

QStringList App::capabilities_for_zome(QString zome){
    if(!m_dna) return QStringList();

    CStringVec names;
    holochain_dna_get_capabilities_names(m_dna, zome.toStdString().c_str(), &names);
    QStringList result;
    for(unsigned int i = 0; i < names.len; i++)
        result << QString("%1").arg(names.ptr[i]);
    return result;
}

QStringList App::functions_for_zome_capability(QString zome, QString capability){
    if(!m_dna) return QStringList();

    CStringVec names;
    holochain_dna_get_function_names(m_dna, zome.toStdString().c_str(), capability.toStdString().c_str(), &names);
    QStringList result;
    for(unsigned int i = 0; i < names.len; i++)
        result << QString("%1").arg(names.ptr[i]);
    return result;
}


QStringList App::parameter_names(QString zome, QString capability, QString function) {
    if(!m_dna) return QStringList();

    CStringVec names;
    holochain_dna_get_function_parameters(m_dna, zome.toStdString().c_str(), capability.toStdString().c_str(), function.toStdString().c_str(), &names);
    QStringList result;
    for(unsigned int i = 0; i < names.len; i++)
        result << QString("%1").arg(names.ptr[i]);
    return result;
}


QString App::hash() const {
    return m_hash;
}

QString App::name() const {
    char *buf2 = holochain_dna_get_name(m_dna);
    QString name(buf2);
    holochain_dna_string_free(buf2);
    return name;
}

App* App::load(QString hash, QString storage_path) {
    Dna *dna = getDna(hash);
    if(!dna) return 0;
    App *app = new App;
    app->m_instance = holochain_new(dna, storage_path.toStdString().c_str());
    // need to recreate a dna object because holochain_new() consumes it..
    // Might wanna change that on the rust side?
    app->m_dna = getDna(hash);
    return app;
}
