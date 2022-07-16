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

bool SettingsStorage::fansProfileExists(const QString &name) const
{
    qSettings->beginGroup("FansProfiles");
    bool result = qSettings->contains(name);
    qSettings->endGroup();
    return result;
}

void SettingsStorage::saveFansProfile(const Fans::Profile &fansProfile)
{
    qSettings->beginGroup("FansProfiles");
    qSettings->setValue(fansProfile.name, QVariant::fromValue(fansProfile));
    qSettings->endGroup();
}

//Fans::Profile SettingsStorage::getFansProfileByName(const QString &name) const
//{
//    qSettings->beginGroup("FansProfiles");
//    auto result = qSettings->contains(name) ? qSettings->value(name).value<Fans::Profile>() : Fans::Profile::Default;
//    qSettings->endGroup();
//    return result;
//}

bool SettingsStorage::cpuProfileExists(const QString &name) const
{
    qSettings->beginGroup("CpuProfiles");
    bool result = qSettings->contains(name);
    qSettings->endGroup();
    return result;
}

void SettingsStorage::saveCpuProfile(const Ryzen::Profile &cpuProfile) const
{
    qSettings->beginGroup("CpuProfiles");
    qSettings->setValue(cpuProfile.name(), cpuProfile.toQStringList());
    qSettings->endGroup();
}

QList<Ryzen::Profile> SettingsStorage::getCpuProfiles() const
{
    qSettings->beginGroup("CpuProfiles");
    QList<Ryzen::Profile> result;
    for (const QString &key : qSettings->allKeys()) {
        result.append(Ryzen::Profile(key, qSettings->value(key).toStringList()));
    }
    qSettings->endGroup();
    return result;
}

//Ryzen::Profile SettingsStorage::getCpuProfileByName(const QString &name) const
//{
//    return Ryzen::Profile::Default;
//}

void SettingsStorage::deleteCpuProfile(const QString &name) const
{
    qSettings->beginGroup("CpuProfiles");
    qSettings->remove(name);
    qSettings->endGroup();
}

QList<Fans::Profile> SettingsStorage::getFansProfiles() const {
    qSettings->beginGroup("FansProfiles");
    QList<Fans::Profile> result;
    for (const QString &key : qSettings->allKeys()) {
        result.append(Fans::Profile(key, qSettings->value(key).toStringList()));
    }
    qSettings->endGroup();
    return result;
}

//Fans::Profile SettingsStorage::getCurrentFansProfile(uchar currentPowerSourceId) const {
//    if (qSettings->contains("currentFansProfile")) {
//        return qSettings->value("currentFansProfile")
//            .value<Fans::Profile>()
//            .withName(FansProfile::CurrentFansProfileName);
//    } else if (qSettings->contains("currentFansProfileName")) {
//        return getFansProfileByName(qSettings->value("currentFansProfileName").value<QString>());
//    } else if (currentPowerProfileExists()) {
//        PowerProfile currentPowerProfile = getCurrentPowerProfile();
//        HwProfile currentHwProfile = currentPowerProfile.getHwProfile(currentPowerSourceId);
//        return currentHwProfile.getFansProfile();
//    } else {
//        return FansProfile::Default;
//    }
//}

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
