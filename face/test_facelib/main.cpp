#include "testsettings.h"


int main(int argc, char *argv[])
{
    TestSettings settings;
    QTest::qExec(&settings, argc, argv);
}

