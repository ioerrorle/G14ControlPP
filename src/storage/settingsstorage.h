#ifndef SETTINGSSTORAGE_H
#define SETTINGSSTORAGE_H

#include <QObject>
#include <QSettings>
#include <QString>

#include <src/model/PowerProfile.h>

class SettingsStorage : public QObject {
public:
    SettingsStorage();

    bool fansProfileExists(const QString &name) const;
    void saveFansProfile(const FansProfile &fansProfile);
    QList<FansProfile> getFansProfiles() const;
    FansProfile getCurrentFansProfile(uchar currentPowerSourceId) const;
    FansProfile getFansProfileByName(const QString &name) const;
    void deleteFansProfile(const QString &name);

    bool cpuProfileExists(const QString &name) const;
    void saveCpuProfile(const Ryzen::Profile &cpuProfile) const;
    QList<Ryzen::Profile> getCpuProfiles() const;
    Ryzen::Profile getCurrentCpuProfile(uchar currentPowerSourceId) const;
    Ryzen::Profile getCpuProfileByName(const QString &name) const;
    void deleteCpuProfile(const QString &name) const;

    bool currentPowerProfileExists() const;
    PowerProfile getCurrentPowerProfile() const;
    PowerProfile getPowerProfileByName(const QString &name) const;

private:
    QSettings *qSettings;
};

#endif // SETTINGSSTORAGE_H
