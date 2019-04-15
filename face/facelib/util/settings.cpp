#include "settings.h"

#include <QSettings>
#include <QMetaEnum>
#include <QRegExp>
#include <QStringList>
#include <QFile>

namespace flib {

/*******************************************************
 *
 *              Settings public
 *
 ********************************************************/

Settings::Settings(){
    const QMetaObject &smo = staticMetaObject;
    int idx = smo.indexOfEnumerator("Key");
    m_keys = smo.enumerator(idx);

    idx = smo.indexOfEnumerator("Section");
    m_sections = smo.enumerator(idx);
}




void Settings::loadFromFile(const QString &name)
{
    QFile file(name);
    file.open(QIODevice::ReadOnly);
    Settings &self = FLIB_SETTINGS;
    self.loadFromRaw(file.readAll());
}

void Settings::loadFromRaw(const QString &data){
    Settings &self = FLIB_SETTINGS;

    //section/key : value
    //section - optional
    QRegExp reqExp("^\\s*(((\\w+)/)?(\\w+))\\s*:\\s*([^\\s].{0,})\\b\\s*$");

    auto kvs = data.split(QRegExp(";\\W*"), QString::SkipEmptyParts); //key-values
    for(auto kv : kvs){
        if(reqExp.indexIn(kv) != -1){
            QString section = reqExp.cap(3);
            if(section.isEmpty()){
                // нет секции, запишется под секцией General
                section = self.m_sections.valueToKey(General);
            }
            QString key = reqExp.cap(4);
            QString value = reqExp.cap(5);

            int iKey = self.m_keys.keyToValue(key.toLocal8Bit().data());
            if(iKey != -1){
                int iSection = self.m_sections.keyToValue(section.toLocal8Bit().data());
                if(iSection != -1){
                    QString keyPath = self.keyPath((Section)iSection,(Key)iKey);
                    self.m_settings[keyPath] = value;
                }
            }
        }
    }
}


QVariant Settings::get(Key k, Section s /*= General*/){
    Settings &self = FLIB_SETTINGS;
    QString key = self.keyPath(s, k);
    if(self.m_settings.contains(key)== true){
        return self.m_settings[key];
    }else{
        return QVariant();
    }
}

/*******************************************************
 *
 *              Settings private
 *
 ********************************************************/
QString Settings::keyPath(Section section, Key key){
    Settings &self = FLIB_SETTINGS;
    auto szSection = self.m_sections.valueToKey(section);
    auto szKey = self.m_keys.valueToKey(key);
    return QString("%1/%2").arg(szSection).arg(szKey);
}

} // flib


