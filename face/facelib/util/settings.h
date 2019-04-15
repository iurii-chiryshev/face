/**
 * @file
 * @brief Заголовочный файл для чтения настроек из файла
 * @author Chiryshev Iurii <iurii.chiryshev@mail.ru>
 * */
#ifndef FLIB_SETTINGS_H
#define FLIB_SETTINGS_H

#include <QVariant>
#include <QSettings>
#include <QMetaEnum>

#include "facelib_global.h"
#include "singleton.h"

namespace flib {

class FACELIBSHARED_EXPORT Settings{
    Q_GADGET
    Q_ENUMS(Section)
    Q_ENUMS(Key)
public:

    /**
     * @brief The Section enum
     * Пары key:value могут лежать в отдельных секциях.
     * Если секция не указано, все лежит в General
     */
    enum Section{
        General, // default section
        Network,
        Proxy
    };

    /**
     * @brief The Key enum
     * Ключи в рамках секций
     */
    enum Key{
        URI,
        Port,
        User,
        Password
    };


    /**
     * @brief loadFromFile
     * @param name
     * загрузить настройки из файла
     */
    void loadFromFile(const QString &name);
    /**
     * @brief loadFromRaw
     * @param data
     * загрузить настройки из строки
     */
    void loadFromRaw(const QString &data);
    /**
     * @brief get
     * @return
     * получить значение по ключу и секции
     */
    QVariant get(Key k, Section s = General);


private:

    QString keyPath(Section section, Key key);

    QMetaEnum m_keys;
    QMetaEnum m_sections;
    QMap<QString, QVariant> m_settings;

    /**
     * @brief Settings
     * ctor
     */
    Settings();
    /**
     * @brief Settings
     * copy ctor
     */
    Settings(const Settings &);

    /**
     * @brief operator =
     * @return
     * not use
     */
    Settings & operator = (const Settings &);

    friend class Singleton<Settings>;
};

#define FLIB_SETTINGS Singleton<Settings>::instance()


}// flib





#endif // FLIB_SETTINGS_H
