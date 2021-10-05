#ifndef G14CONTROLPP_SETTINGS_H
#define G14CONTROLPP_SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QApplication>
#include "src/atkacpi/AcpiControlSingleton.h"

#define SETT Settings::getInstance()

class Settings : public QObject {

public:
    static Settings &getInstance();

private:
    Settings();
    QSettings *qSettings;

    void saveFansProfiles(QList<FansProfile> &profiles);

public:
    Settings(Settings const &) = delete;
    void operator=(Settings const &) = delete;

    void putKbdBr(uchar value);
    uchar getKbdBr();

    void saveFansProfile(FansProfile &profile);
    void deleteFansProfile(FansProfile &profile);
    QList<FansProfile> getFansProfiles();
};


#endif //G14CONTROLPP_SETTINGS_H
