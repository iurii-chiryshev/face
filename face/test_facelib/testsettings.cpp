#include "testsettings.h"

#include <util/settings.h>

using namespace flib;

TestSettings::TestSettings()
{

}

void TestSettings::invoke()
{
    Settings &config = FLIB_SETTINGS;
    config.loadFromRaw("General/URI: http://yandex.ru;\n Port: 8088;\n User: user;\n Password: 12345;\n");

    QCOMPARE(config.get(Settings::URI).toString(),QString("http://yandex.ru"));
    QCOMPARE(config.get(Settings::Port).toInt(),8088);
    QCOMPARE(config.get(Settings::User).toString(),QString("user"));
    QCOMPARE(config.get(Settings::Password).toString(),QString("12345"));
}
