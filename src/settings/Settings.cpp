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

bool Settings::saveFansProfile(FansProfile &profile, bool override) {
    AcpiControlSingleton::fixFanCurve(FAN_CPU, profile.cpu);
    AcpiControlSingleton::fixFanCurve(FAN_GPU, profile.gpu);

    auto currentProfiles = getFansProfiles();
    for (int i = 0; i < currentProfiles.size(); i++) {
        if (profile.name == currentProfiles[i].name) {
            if (!override) {
                return false;
            } else {
                currentProfiles.replace(i, profile);
                return true;
            }
        }
    }

    currentProfiles.append(profile);
    saveFansProfiles(currentProfiles);
    return true;
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

void Settings::setCurrentPowerPlan(uchar id) {
    qSettings->setValue("current_power_plan", id);
}

ArmouryCratePowerPlan Settings::getCurrentPowerPlan() {
    uchar powerPlanId = qSettings->value("current_power_plan", 0).value<bool>();
    return POWER_PLANS[powerPlanId];
}

void Settings::setUseDefaultFanCurves(bool value) {
    qSettings->setValue("use_default_fan_curves", value);
}

bool Settings::getUseDefaultFanCurves() {
    return qSettings->value("use_default_fan_curves", true).value<bool>();
}

void Settings::setCurrentFanCurveProfile(FansProfile &profile) {
    if (!profile.name.isEmpty()) {
        qSettings->setValue("current_fan_profile_name", profile.name);
    } else {
        qSettings->remove("current_fan_profile_name");
    }
    qSettings->setValue("current_fan_profile", QVariant::fromValue(profile));
}

FansProfile Settings::getCurrentFanCurveProfile() {
    auto name = qSettings->value("current_fan_profile_name").value<QString>();
    if (!name.isEmpty()) {
        auto savedFanCurves = getFansProfiles();
        for (FansProfile saved : savedFanCurves) {
            if (saved.name == name) {
                return saved;
            }
        }
    }

    FansProfile result = {};
    if (qSettings->contains("current_fan_profile")) {
        result = qSettings->value("current_fan_profile").value<FansProfile>();
    }
    AcpiControlSingleton::fixFanCurve(FAN_CPU, result.cpu);
    AcpiControlSingleton::fixFanCurve(FAN_GPU, result.gpu);
    return result;
}
