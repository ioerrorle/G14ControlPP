#ifndef G14CONTROLPP_SETTINGS_H
#define G14CONTROLPP_SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QApplication>
#include "src/atkacpi/AcpiControlSingleton.h"
#include "src/ryzenadj/RyzenAdjTypes.h"
#include "src/ryzenadj/PowerProfile.h"

typedef struct {
    uchar armouryCratePlanId;
    FansProfile fansProfile;
    PowerProfile powerProfile;
} PowerPlan;

bool operator==(const PowerPlan& lhs, const PowerPlan& rhs);
bool operator<(const PowerPlan& l, const PowerPlan& r);
QDataStream &operator<<(QDataStream &out, const PowerPlan &v);
QDataStream &operator>>(QDataStream &in, PowerPlan &v);

Q_DECLARE_METATYPE(PowerPlan);

#define SETT Settings::getInstance()

class Settings : public QObject {

public:
    static Settings &getInstance();

private:
    Settings();
    QSettings *qSettings;

    void saveFansProfiles(QList<FansProfile> &profiles);
    void savePowerProfiles(QList<PowerProfile> &profiles);

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
};


#endif //G14CONTROLPP_SETTINGS_H
