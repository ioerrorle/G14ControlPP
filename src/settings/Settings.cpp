#include "Settings.h"

QDataStream &operator<<(QDataStream &out, const PowerPlan &v) {
    out << v.armouryCratePlanId;
    out << v.fansProfile;
    out << v.powerProfile;
    return out;
}

QDataStream &operator>>(QDataStream &in, PowerPlan &v) {
    in >> v.armouryCratePlanId;
    in >> v.fansProfile;
    in >> v.powerProfile;
    return in;
}

bool operator==(const PowerPlan& lhs, const PowerPlan& rhs){
    return lhs.powerProfile == rhs.powerProfile
           && lhs.fansProfile == rhs.fansProfile
           && lhs.armouryCratePlanId == rhs.armouryCratePlanId;
}

bool operator<(const PowerPlan &l, const PowerPlan &r) {
    return l.powerProfile < r.powerProfile
           || l.fansProfile < r.fansProfile
           || l.armouryCratePlanId < r.armouryCratePlanId;
}

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

ArmouryCratePlan Settings::getCurrentPowerPlan() {
    uchar powerPlanId = qSettings->value("current_power_plan", 0).value<bool>();
    return ARMOURY_CRATE_PLANS[powerPlanId];
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

void Settings::savePowerProfiles(QList<PowerProfile> &profiles) {
    qSettings->setValue("power_profiles_list", QVariant::fromValue(profiles));
}

bool Settings::savePowerProfile(PowerProfile &powerProfile, bool override) {
    auto currentProfiles = getPowerProfiles(false);
    for (int i = 0; i < currentProfiles.size(); i++) {
        if (powerProfile.name == currentProfiles[i].name) {
            if (!override) {
                return false;
            } else {
                currentProfiles.replace(i, powerProfile);
                return true;
            }
        }
    }

    currentProfiles.append(powerProfile);
    savePowerProfiles(currentProfiles);
    return true;
}

void Settings::deletePowerProfile(PowerProfile &powerProfile) {
    auto currentProfiles = getPowerProfiles(false);

    for (int i = 0; i < currentProfiles.size(); i++) {
        if (currentProfiles[i].name == powerProfile.name) {
            currentProfiles.removeAt(i);
            break;
        }
    }

    savePowerProfiles(currentProfiles);
}

QList<PowerProfile> Settings::getPowerProfiles(bool includeStock) {
    QList<PowerProfile> result;
    if (!qSettings->contains("power_profiles_list")) {
        result = QList<PowerProfile>();
    } else {
        result = qSettings->value("power_profiles_list").value<QList<PowerProfile>>();
    }
    if (includeStock) {
        result.insert(0, STOCK_PROFILE);
    }
    return result;
}

void Settings::setCurrentPowerProfile(PowerProfile &powerProfile) {
    if (!powerProfile.name.isEmpty()) {
        qSettings->setValue("current_power_profile_name", powerProfile.name);
    } else {
        qSettings->remove("current_power_profile_name");
    }
    qSettings->setValue("current_power_profile", QVariant::fromValue(powerProfile));
}

PowerProfile Settings::getCurrentPowerProfile() {
    auto name = qSettings->value("current_power_profile_name").value<QString>();
    if (!name.isEmpty()) {
        auto savedPowerProfiles = getPowerProfiles(true);
        for (PowerProfile saved : savedPowerProfiles) {
            if (saved.name == name) {
                return saved;
            }
        }
    }

    if (qSettings->contains("current_power_profile")) {
        return qSettings->value("current_power_profile").value<PowerProfile>();
    } else {
        return STOCK_PROFILE;
    }
}

void Settings::putMaxBatteryCharge(uchar value) {
    qSettings->setValue("battery_max_charge", value);
}

uchar Settings::getMaxBatteryCharge() {
    return qSettings->value("battery_max_charge", 100).value<uchar>();
}

void Settings::savePowerPlans(PowerPlan &dcPowerPlan, PowerPlan &acPowerPlan, PowerPlan &usbPowerPlan) {
    qSettings->setValue("dc_power_plan", QVariant::fromValue(dcPowerPlan));
    qSettings->setValue("ac_power_plan", QVariant::fromValue(acPowerPlan));
    qSettings->setValue("usb_power_plan", QVariant::fromValue(usbPowerPlan));
}

void Settings::loadPowerPlans(PowerPlan &dcPowerPlan, PowerPlan &acPowerPlan, PowerPlan &usbPowerPlan) {
    if (qSettings->contains("dc_power_plan")) {
        dcPowerPlan = qSettings->value("dc_power_plan").value<PowerPlan>();
    } else {
        dcPowerPlan = {0, {}, {}};
    }
    if (qSettings->contains("ac_power_plan")) {
        acPowerPlan = qSettings->value("ac_power_plan").value<PowerPlan>();
    } else {
        acPowerPlan = {0, {}, {}};
    }
    if (qSettings->contains("usb_power_plan")) {
        usbPowerPlan = qSettings->value("usb_power_plan").value<PowerPlan>();
    } else {
        usbPowerPlan = {0, {}, {}};
    }
}

PowerPlan Settings::loadPowerPlanForPowerSource(PowerSourceType &source) {
    QString key;
    switch (source) {
        case POWER_SOURCE_BATTERY:
            key = "dc_power_plan";
            break;
        case POWER_SOURCE_180W:
            key = "ac_power_plan";
            break;
        case POWER_SOURCE_USB:
            key = "usb_power_plan";
            break;
    }

    if (qSettings->contains(key)) {
        return qSettings->value(key).value<PowerPlan>();
    } else {
        return {0, {}, {}};
    }
}
