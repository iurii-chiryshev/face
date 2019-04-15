#ifndef TESTSETTINGS_H
#define TESTSETTINGS_H

#include <QtTest>
#include <QString>



class TestSettings : public QObject
{
    Q_OBJECT

public:
    TestSettings();

private Q_SLOTS:
    void invoke();
};



#endif // TESTSETTINGS_H
