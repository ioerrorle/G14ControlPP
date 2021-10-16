#ifndef G14CONTROLPP_SETTINGS_H
#define G14CONTROLPP_SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QApplication>
#include "src/atkacpi/AcpiControlSingleton.h"
#include "src/ryzenadj/RyzenAdjTypes.h"
#include "src/ryzenadj/PowerProfile.h"
#include "PowerPlanSet.h"

#define SETT Settings::getInstance()

class Settings : public QObject {

public:
    static Settings &getInstance();

private:
    Settings();
    QSettings *qSettings;

    void saveFansProfiles(QList<FansProfile> &profiles);
    void savePowerProfiles(QList<PowerProfile> &profiles);
    void savePowerPlanSets(QList<PowerPlanSet> &powerPlanSets);

public:
    Settings(Settings const &) = delete;
    void operator=(Settings const &) = delete;

    void putKbdBr(uchar value);
    uchar getKbdBr();

    bool saveFansProfile(FansProfile &profile, bool override = true);
    void deleteFansProfile(FansProfile &profile);
    QList<FansProfile> getFansProfiles();

    void setCurrentPowerPlan(uchar id);
    ArmouryCratePlan getCurrentPowerPlan();

    void setUseDefaultFanCurves(bool value);
    bool getUseDefaultFanCurves();

    void setCurrentFanCurveProfile(FansProfile &profile);
    FansProfile getCurrentFanCurveProfile();

    bool savePowerProfile(PowerProfile &powerProfile, bool override = true);
    void deletePowerProfile(PowerProfile &powerProfile);
    QList<PowerProfile> getPowerProfiles(bool includeStock = true);

    void setCurrentPowerProfile(PowerProfile &powerProfile);
    PowerProfile getCurrentPowerProfile();

    void putMaxBatteryCharge(const uchar value);
    uchar getMaxBatteryCharge();

    void savePowerPlans(PowerPlan &dcPowerPlan, PowerPlan &acPowerPlan, PowerPlan &usbPowerPlan);
    void loadPowerPlans(PowerPlan &dcPowerPlan, PowerPlan &acPowerPlan, PowerPlan &usbPowerPlan);
    PowerPlan loadPowerPlanForPowerSource(PowerSourceType &source);

    bool savePowerPlanSet(PowerPlanSet &powerPlanSet, bool override = true);
    void deletePowerPlanSet(PowerPlanSet &powerPlanSet);
    QList<PowerPlanSet> getPowerPlanSets(bool includeStock = true);

    void setUsedPowerPlans(QStringList &list);
    QStringList getUsedPowerPlans();
    QStringList getUsedPowerPlans(QList<PowerPlanSet> &powerPlanSets);

    void setCurrentPowerPlanSetName(const QString &name);
    QString getCurrentPowerPlanSetName();

    PowerPlanSet getPowerPlanSetByName(QString name);

    PowerPlanSet getCurrentPowerPlanSet();
};


#endif //G14CONTROLPP_SETTINGS_H
