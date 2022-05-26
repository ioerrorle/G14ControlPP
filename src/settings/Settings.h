#ifndef G14CONTROLPP_SETTINGS_H
#define G14CONTROLPP_SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QApplication>
#include "PowerPlanSet.h"
#include "src/ui/hotkeys/HotkeysProfile.h"

#define SETT Settings::getInstance()

class Settings : public QObject {

public:
    static Settings &getInstance();

private:
    Settings();
    QSettings *qSettings;

public:
    Settings(Settings const &) = delete;
    void operator=(Settings const &) = delete;

    void putKbdBr(uchar value);
    uchar getKbdBr();

    bool saveFansProfile(FansProfile &fansProfile, bool override = true);
    void deleteFansProfile(const QString &name);
    QList<FansProfile> getFansProfiles();
    FansProfile getFansProfile(const QString &name);

    void setCurrentPowerPlan(uchar id);
    ArmouryCratePlan getCurrentPowerPlan();

    void setUseDefaultFanCurves(bool value);
    bool getUseDefaultFanCurves();

    void setCurrentFanCurveProfile(FansProfile &profile);
    FansProfile getCurrentFanCurveProfile();

    bool savePowerProfile(CpuProfile &powerProfile, bool override = true);
    void deletePowerProfile(const QString &powerProfileName);
    CpuProfile getPowerProfile(const QString &name);
    QList<CpuProfile> getPowerProfiles();

    void setCurrentPowerProfile(CpuProfile &powerProfile);
    CpuProfile getCurrentPowerProfile();

    void putMaxBatteryCharge(const uchar value);
    uchar getMaxBatteryCharge();

    bool savePowerPlanSet(PowerPlanSet &powerPlanSet, bool override = true);
    void deletePowerPlanSet(const QString &powerPlanSetName);
    QList<PowerPlanSet> getPowerPlanSets(bool includeStock = true);

    void setUsedPowerPlans(QStringList &list);
    QStringList getUsedPowerPlans();
    QStringList getUsedPowerPlans(QList<PowerPlanSet> &powerPlanSets);

    void setCurrentPowerPlanSetName(const QString &name);
    QString getCurrentPowerPlanSetName();

    PowerPlanSet getPowerPlanSet(const QString &name);

    PowerPlanSet getCurrentPowerPlanSet();

    QList<HotkeysProfile> getHotkeysProfiles();
    HotkeysProfile getCurrentHotkeysProfile();
    void setCurrentHotkeysProfileName(const QString &name);
    bool saveHotkeysProfile(HotkeysProfile &hotkeysProfile, bool override);
    void deleteHotkeysProfile(const QString &hotkeysProfileName);
};


#endif //G14CONTROLPP_SETTINGS_H
