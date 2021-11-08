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

bool Settings::saveFansProfile(FansProfile &fansProfile, bool override) {
    bool result;
    qSettings->beginGroup("FansProfiles");
    if (qSettings->contains(fansProfile.getName()) && !override) {
        result = false;
    } else {
        qSettings->setValue(fansProfile.getName(), fansProfile.toQStringList());
        result = true;
    }
    qSettings->endGroup();
    return result;
}

void Settings::deleteFansProfile(const QString &name) {
    qSettings->beginGroup("FansProfiles");
    qSettings->remove(name);
    qSettings->endGroup();
}

QList<FansProfile> Settings::getFansProfiles() {
    qSettings->beginGroup("FansProfiles");
    QList<FansProfile> result;
    for (const QString &key : qSettings->allKeys()) {
        result.append(FansProfile::fromQStringList(key, qSettings->value(key).toStringList()));
    }
    qSettings->endGroup();
    return result;
}

FansProfile Settings::getFansProfile(const QString &name) {
    qSettings->beginGroup("FansProfiles");
    if (qSettings->contains(name)) {
        auto result = FansProfile::fromQStringList(name, qSettings->value(name).toStringList());
        qSettings->endGroup();
        return result;
    } else {
        qSettings->endGroup();
        return FansProfile();
    }
}

void Settings::setCurrentPowerPlan(uchar id) {
    qSettings->setValue("current_power_plan", id);
}

ArmouryCratePlan Settings::getCurrentPowerPlan() {
    uchar powerPlanId = qSettings->value("current_power_plan", 0).value<bool>();
    return ArmouryCratePlan::plans()[powerPlanId];
}

void Settings::setUseDefaultFanCurves(bool value) {
    qSettings->setValue("use_default_fan_curves", value);
}

bool Settings::getUseDefaultFanCurves() {
    return qSettings->value("use_default_fan_curves", true).value<bool>();
}

void Settings::setCurrentFanCurveProfile(FansProfile &profile) {
    if (!profile.isEmpty()) {
        qSettings->setValue("current_fan_profile_name", profile.getName());
    } else {
        qSettings->remove("current_fan_profile_name");
    }
    qSettings->setValue("current_fan_profile", QVariant::fromValue(profile));
}

FansProfile Settings::getCurrentFanCurveProfile() {
    auto name = qSettings->value("current_fan_profile_name").value<QString>();
    if (!name.isEmpty()) {
        return getFansProfile(name);
    } else {
        return FansProfile();
    }
}

bool Settings::savePowerProfile(PowerProfile &powerProfile, bool override) {
    bool result;
    qSettings->beginGroup("PowerProfiles");
    if (qSettings->contains(powerProfile.getName()) && !override) {
        result = false;
    } else {
        qSettings->setValue(powerProfile.getName(), powerProfile.toQStringList());
        result = true;
    }
    qSettings->endGroup();
    return result;
}

void Settings::deletePowerProfile(QString &name) {
    qSettings->beginGroup("PowerProfiles");
    qSettings->remove(name);
    qSettings->endGroup();
}

PowerProfile Settings::getPowerProfile(QString &name) {
    qSettings->beginGroup("PowerProfiles");
    if (qSettings->contains(name)) {
        auto result = PowerProfile::fromQStringList(name, qSettings->value(name).toStringList());
        qSettings->endGroup();
        return result;
    } else {
        qSettings->endGroup();
        return PowerProfile();
    }
}

QList<PowerProfile> Settings::getPowerProfiles() {
    qSettings->beginGroup("PowerProfiles");
    QList<PowerProfile> result;
    for (const QString &key : qSettings->allKeys()) {
        result.append(PowerProfile::fromQStringList(key, qSettings->value(key).toStringList()));
    }
    qSettings->endGroup();
    return result;
}

void Settings::setCurrentPowerProfile(PowerProfile &powerProfile) {
    if (powerProfile.isEmpty()) {
        qSettings->setValue("current_power_profile_name", powerProfile.getName());
    } else {
        qSettings->remove("current_power_profile_name");
    }
}

PowerProfile Settings::getCurrentPowerProfile() {
    auto name = qSettings->value("current_power_profile_name").value<QString>();
    if (!name.isEmpty()) {
        return getPowerProfile(name);
    } else {
        return PowerProfile();
    }
}

void Settings::putMaxBatteryCharge(uchar value) {
    qSettings->setValue("battery_max_charge", value);
}

uchar Settings::getMaxBatteryCharge() {
    return qSettings->value("battery_max_charge", 100).value<uchar>();
}

bool Settings::savePowerPlanSet(PowerPlanSet &powerPlanSet, bool override) {
    bool result;
    qSettings->beginGroup("PowerPlanSets");
    if (qSettings->contains(powerPlanSet.getName()) && !override) {
        result = false;
    } else {
        qSettings->setValue(powerPlanSet.getName(), powerPlanSet.toQStringList());
        result = true;
    }
    qSettings->endGroup();
    return result;
}

void Settings::deletePowerPlanSet(const QString &powerPlanSetName) {
    qSettings->beginGroup("PowerPlanSets");
    qSettings->remove(powerPlanSetName);
    qSettings->endGroup();
}

QList<PowerPlanSet> Settings::getPowerPlanSets(bool includeDefault) {
    qSettings->beginGroup("PowerPlanSets");
    QList<PowerPlanSet> result;
    for (const QString &key : qSettings->allKeys()) {
        result.append(PowerPlanSet::fromQStringList(key, qSettings->value(key).toStringList()));
    }
    qSettings->endGroup();
    return result;
}

PowerPlanSet Settings::getPowerPlanSet(const QString &name) {
    qSettings->beginGroup("PowerPlanSets");
    if (qSettings->contains(name)) {
        auto result = PowerPlanSet::fromQStringList(name, qSettings->value(name).toStringList());
        qSettings->endGroup();
        return result;
    } else {
        qSettings->endGroup();
        return PowerPlanSet();
    }
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
    for (const PowerPlanSet &set : powerPlanSets) {
        powerPlanSetsNames.append(set.getName());
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
        powerPlanSetsNames.append(set.getName());
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

PowerPlanSet Settings::getCurrentPowerPlanSet() {
    return getPowerPlanSet(getCurrentPowerPlanSetName());
}

void Settings::setCurrentPowerPlanSetName(const QString &name) {
    qSettings->setValue("current_power_plan_set_name", name);
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
    if (qSettings->contains(currentHotkeyProfileName)) {
        qSettings->endGroup();
        return qSettings->value(currentHotkeyProfileName).value<HotkeysProfile>();
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

