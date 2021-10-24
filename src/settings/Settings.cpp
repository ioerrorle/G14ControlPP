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
                saveFansProfiles(currentProfiles);
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
    auto currentProfiles = getPowerProfiles();
    for (int i = 0; i < currentProfiles.size(); i++) {
        if (powerProfile.name == currentProfiles[i].name) {
            if (!override) {
                return false;
            } else {
                currentProfiles.replace(i, powerProfile);
                savePowerProfiles(currentProfiles);
                return true;
            }
        }
    }

    currentProfiles.append(powerProfile);
    savePowerProfiles(currentProfiles);
    return true;
}

void Settings::deletePowerProfile(PowerProfile &powerProfile) {
    auto currentProfiles = getPowerProfiles();

    for (int i = 0; i < currentProfiles.size(); i++) {
        if (currentProfiles[i].name == powerProfile.name) {
            currentProfiles.removeAt(i);
            break;
        }
    }

    savePowerProfiles(currentProfiles);
}

QList<PowerProfile> Settings::getPowerProfiles() {
    QList<PowerProfile> result;
    if (!qSettings->contains("power_profiles_list")) {
        result = QList<PowerProfile>();
    } else {
        result = qSettings->value("power_profiles_list").value<QList<PowerProfile>>();
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
        auto savedPowerProfiles = getPowerProfiles();
        for (PowerProfile saved : savedPowerProfiles) {
            if (saved.name == name) {
                return saved;
            }
        }
    }

    if (qSettings->contains("current_power_profile")) {
        return qSettings->value("current_power_profile").value<PowerProfile>();
    } else {
        return STOCK_POWER_PROFILE;
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

void Settings::savePowerPlanSets(QList<PowerPlanSet> &powerPlanSets) {
    qSettings->setValue("power_plan_set_list", QVariant::fromValue(powerPlanSets));
}

bool Settings::savePowerPlanSet(PowerPlanSet &powerPlanSet, bool override) {
    auto currentPowerPlanSets = getPowerPlanSets(false);
    for (int i = 0; i < currentPowerPlanSets.size(); i++) {
        if (powerPlanSet.name == currentPowerPlanSets[i].name) {
            if (!override) {
                return false;
            } else {
                currentPowerPlanSets.replace(i, powerPlanSet);
                savePowerPlanSets(currentPowerPlanSets);
                return true;
            }
        }
    }

    currentPowerPlanSets.append(powerPlanSet);
    savePowerPlanSets(currentPowerPlanSets);
    return true;
}

void Settings::deletePowerPlanSet(PowerPlanSet &powerPlanSet) {
    auto currentPowerPlanSets = getPowerPlanSets(false);

    for (int i = 0; i < currentPowerPlanSets.size(); i++) {
        if (currentPowerPlanSets[i].name == powerPlanSet.name) {
            currentPowerPlanSets.removeAt(i);
            break;
        }
    }

    savePowerPlanSets(currentPowerPlanSets);
}

QList<PowerPlanSet> Settings::getPowerPlanSets(bool includeDefault) {
    QList<PowerPlanSet> result;
    if (!qSettings->contains("power_plan_set_list")) {
        result = QList<PowerPlanSet>();
    } else {
        result = qSettings->value("power_plan_set_list").value<QList<PowerPlanSet>>();
    }
    if (includeDefault) {
        result.insert(0, {"Default"});
    }
    return result;
}

void Settings::setUsedPowerPlans(QStringList &list) {
    if (!list.empty()) {
        qSettings->setValue("used_power_plans_list", list);
    } else {
        qSettings->remove("used_power_plans_list");
    }
}

QStringList Settings::getUsedPowerPlans() {
    QStringList result;
    if (!qSettings->contains("used_power_plans_list")) {
        return QStringList();
    } else {
        result = qSettings->value("used_power_plans_list").value<QStringList>();
    }

    auto powerPlanSets = getPowerPlanSets(true);
    QStringList powerPlanSetsNames;
    for (const PowerPlanSet& set : powerPlanSets) {
        powerPlanSetsNames.append(set.name);
    }

    for (int i = 0; i < result.size(); i++) {
        if (!powerPlanSetsNames.contains(result[i])) {
            result.removeAt(i);
            i--;
        }
    }

    return result;
}

QStringList Settings::getUsedPowerPlans(QList<PowerPlanSet> &powerPlanSets) {
    QStringList result;
    if (!qSettings->contains("used_power_plans_list")) {
        return QStringList();
    } else {
        result = qSettings->value("used_power_plans_list").value<QStringList>();
    }

    QStringList powerPlanSetsNames;
    for (const PowerPlanSet &set : powerPlanSets) {
        powerPlanSetsNames.append(set.name);
    }

    for (int i = 0; i < result.size(); i++) {
        if (!powerPlanSetsNames.contains(result[i])) {
            result.removeAt(i);
            i--;
        }
    }

    return result;
}

QString Settings::getCurrentPowerPlanSetName() {
    if (!qSettings->contains("current_power_plan_set_name")) {
        return "";
    }
    return qSettings->value("current_power_plan_set_name").value<QString>();
}

PowerPlanSet Settings::getPowerPlanSetByName(QString name) {
    auto powerPlanSets = getPowerPlanSets(true);
    for (PowerPlanSet set : powerPlanSets) {
        if (set.name == name) {
            return set;
        }
    }
    PowerPlanSet result = {};
    return result;
}

PowerPlanSet Settings::getCurrentPowerPlanSet() {
    return getPowerPlanSetByName(getCurrentPowerPlanSetName());
}

void Settings::setCurrentPowerPlanSetName(const QString &name) {
    qSettings->setValue("current_power_plan_set_name", name);
}

void Settings::saveHotkeysProfiles(QList<HotkeysProfile> &profiles) {
    qSettings->beginGroup("Hotkeys");
    for (HotkeysProfile &profile : profiles) {
        qSettings->setValue(profile.name, QVariant::fromValue(profile));
    }
    qSettings->endGroup();
}

QList<HotkeysProfile> Settings::getHotkeysProfiles() {
    qSettings->beginGroup("Hotkeys");
    auto result = QList<HotkeysProfile>();
    for (QString &key : qSettings->allKeys()) {
        result.append(qSettings->value(key).value<HotkeysProfile>());
    }
    if (!qSettings->allKeys().contains("Coder")) {
        result.insert(0, HOTKEY_PROFILE_CODER);
    }
    if (!qSettings->allKeys().contains("Default")) {
        result.insert(0, HOTKEY_PROFILE_DEFAULT);
    }
    qSettings->endGroup();
    return result;
}

HotkeysProfile Settings::getCurrentHotkeysProfile() {
    auto currentHotkeyProfileName = qSettings->value("current_hotkeys_profile").value<QString>();
    if (currentHotkeyProfileName.isEmpty()) {
        return HOTKEY_PROFILE_DEFAULT;
    }
    qSettings->beginGroup("Hotkeys");
    auto allKeys = qSettings->allKeys();
    //searching for current profile
    for (QString &key : allKeys) {
        if (key == currentHotkeyProfileName) {
            auto result = qSettings->value(key).value<HotkeysProfile>();
            qSettings->endGroup();
            return result;
        }
    }
    qSettings->endGroup();
    //check if it's hardcoded profile
    if (currentHotkeyProfileName == HOTKEY_PROFILE_CODER.name) {
        return HOTKEY_PROFILE_CODER;
    }
    if (currentHotkeyProfileName == HOTKEY_PROFILE_DEFAULT.name) {
        return HOTKEY_PROFILE_DEFAULT;
    }
    //not found
    qSettings->remove("current_hotkeys_profile");
    return HOTKEY_PROFILE_DEFAULT;
}

void Settings::setCurrentHotkeysProfileName(const QString &name) {
    if (name.isEmpty()) {
        qSettings->remove("current_hotkeys_profile");
    } else {
        qSettings->setValue("current_hotkeys_profile", name);
    }
}

bool Settings::saveHotkeysProfile(HotkeysProfile &hotkeysProfile, bool override) {
    if (override) {
        qSettings->beginGroup("Hotkeys");
        qSettings->setValue(hotkeysProfile.name, QVariant::fromValue(hotkeysProfile));
        qSettings->endGroup();
        return true;
    } else {
        qSettings->beginGroup("Hotkeys");
        if (qSettings->contains(hotkeysProfile.name)) {
            qSettings->endGroup();
            return false;
        } else {
            qSettings->setValue(hotkeysProfile.name, QVariant::fromValue(hotkeysProfile));
            qSettings->endGroup();
            return true;
        }
    }
}

void Settings::deleteHotkeysProfile(const QString &hotkeysProfileName) {
    qSettings->beginGroup("Hotkeys");
    qSettings->remove(hotkeysProfileName);
    qSettings->endGroup();
}

