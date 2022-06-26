#include "settingsstorage.h"
#include <QCoreApplication>

SettingsStorage::SettingsStorage() {
    /*
    QCoreApplication::setOrganizationName("IoError LTD");
    QCoreApplication::setOrganizationDomain("ioerror.pro");
    QCoreApplication::setApplicationName("G14ControlPP");
    */

    this->qSettings = new QSettings("config.ini", QSettings::IniFormat);
}

bool SettingsStorage::fansProfileExists(const QString &name) const {
    qSettings->beginGroup("FansProfiles");
    bool result = qSettings->contains(name);
    qSettings->endGroup();
    return result;
}

void SettingsStorage::saveFansProfile(const FansProfile &fansProfile) {
    qSettings->beginGroup("FansProfiles");
    qSettings->setValue(fansProfile.getName(), QVariant::fromValue(fansProfile));
    qSettings->endGroup();
}

FansProfile SettingsStorage::getFansProfileByName(const QString &name) const {
    qSettings->beginGroup("FansProfiles");
    auto result = qSettings->contains(name) ? qSettings->value(name).value<FansProfile>() : FansProfile::Default;
    qSettings->endGroup();
    return result;
}

bool SettingsStorage::cpuProfileExists(const QString &name) const
{
    qSettings->beginGroup("CpuProfiles");
    bool result = qSettings->contains(name);
    qSettings->endGroup();
    return result;
}

void SettingsStorage::saveCpuProfile(const CpuProfile &cpuProfile) const
{
    qSettings->beginGroup("CpuProfiles");
    qSettings->setValue(cpuProfile.name(), cpuProfile.toQStringList());
    qSettings->endGroup();
}

QList<CpuProfile> SettingsStorage::getCpuProfiles() const
{
    qSettings->beginGroup("CpuProfiles");
    QList<CpuProfile> result;
    for (const QString &key : qSettings->allKeys()) {
        result.append(CpuProfile::fromQStringList(key, qSettings->value(key).toStringList()));
    }
    qSettings->endGroup();
    return result;
}

CpuProfile SettingsStorage::getCpuProfileByName(const QString &name) const
{
    return CpuProfile::Default;
}

void SettingsStorage::deleteCpuProfile(const QString &name) const
{
    qSettings->beginGroup("CpuProfiles");
    qSettings->remove(name);
    qSettings->endGroup();
}

QList<FansProfile> SettingsStorage::getFansProfiles() const {
    qSettings->beginGroup("FansProfiles");
    QList<FansProfile> result;
    for (const QString &key : qSettings->allKeys()) {
        result.append(FansProfile::fromQStringList(key, qSettings->value(key).toStringList()));
    }
    qSettings->endGroup();
    return result;
}

FansProfile SettingsStorage::getCurrentFansProfile(uchar currentPowerSourceId) const {
    if (qSettings->contains("currentFansProfile")) {
        return qSettings->value("currentFansProfile")
            .value<FansProfile>()
            .withName(FansProfile::CurrentFansProfileName);
    } else if (qSettings->contains("currentFansProfileName")) {
        return getFansProfileByName(qSettings->value("currentFansProfileName").value<QString>());
    } else if (currentPowerProfileExists()) {
        PowerProfile currentPowerProfile = getCurrentPowerProfile();
        HwProfile currentHwProfile = currentPowerProfile.getHwProfile(currentPowerSourceId);
        return currentHwProfile.getFansProfile();
    } else {
        return FansProfile::Default;
    }
}

bool SettingsStorage::currentPowerProfileExists() const {
    return qSettings->contains("currentPowerProfile")
           || qSettings->contains("currentPowerProfilename");
}

PowerProfile SettingsStorage::getCurrentPowerProfile() const {
    if (qSettings->contains("currentPowerProfile")) {
        return qSettings->value("currentPowerProfile").value<PowerProfile>();
    } else if (qSettings->contains("currentPowerProfileName")) {
        return getPowerProfileByName(qSettings->value("currentPowerProfileName").value<QString>());
    } else {
        return PowerProfile::Default;
    }
}

PowerProfile SettingsStorage::getPowerProfileByName(const QString &name) const {
    qSettings->beginGroup("FansProfiles");
    auto result = qSettings->contains(name) ? qSettings->value(name).value<PowerProfile>()
                                            : PowerProfile::Default;
    qSettings->endGroup();
    return result;
}
