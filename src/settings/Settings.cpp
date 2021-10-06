#include "Settings.h"

Settings &Settings::getInstance() {
    static Settings instance;
    return instance;
}

Settings::Settings() {
    QCoreApplication::setOrganizationName("IoError LTD");
    QCoreApplication::setOrganizationDomain("ioerror.pro");
    QCoreApplication::setApplicationName("G14ControlPP");

    this->qSettings = new QSettings("config.ini", QSettings::IniFormat);
}

void Settings::putKbdBr(uchar value) {
    qSettings->setValue(tr("keyboard_brightness"), value);
}

uchar Settings::getKbdBr() {
    return qSettings->value(tr("keyboard_brightness"), 0).value<unsigned char>();
}

void Settings::saveFansProfile(FansProfile &profile) {
    AcpiControlSingleton::fixFanCurve(FAN_CPU, profile.cpu);
    AcpiControlSingleton::fixFanCurve(FAN_GPU, profile.gpu);

    auto currentProfiles = getFansProfiles();
    currentProfiles.append(profile);

    saveFansProfiles(currentProfiles);
}

void Settings::deleteFansProfile(FansProfile &profile) {
    auto currentProfiles = getFansProfiles();

    for (int i = 0; i < currentProfiles.size(); i++) {
        if (currentProfiles[i].name == profile.name) {
            currentProfiles.removeAt(i);
            break;
        }
    }

    saveFansProfiles(currentProfiles);
}

QList<FansProfile> Settings::getFansProfiles() {
    if (!qSettings->contains("fan_profiles_list")) {
        return QList<FansProfile>();
    }
    return qSettings->value("fan_profiles_list").value<QList<FansProfile>>();
}

void Settings::saveFansProfiles(QList<FansProfile> &profiles) {
    qSettings->setValue("fan_profiles_list", QVariant::fromValue(profiles));
}
