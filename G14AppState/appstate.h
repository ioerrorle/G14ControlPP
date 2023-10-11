#ifndef G14APPSTATE_H
#define G14APPSTATE_H

#include "PowerSource/powersource.h"
#include "G14ArmouryCrate/g14armourycrate.h"
#include "CpuProfile/cpuprofile.h"
#include "FanCurve/fancurve.h"

struct PowerProfile
{
    G14ArmouryCratePlan arCratePlan;
    FanCurve cpuFanCurve;
    FanCurve gpuFanCurve;
    CpuProfile cpuProfile;
};

struct FullPowerProfile
{
    QString name;
    QMap<PowerSource, PowerProfile> powerProfiles;
};

struct KeyboardBklState
{
    int lightLevel = 0;
    bool isLightOn = true;
};

enum class FnKey
{

};

enum class KeyboardActionType
{

};

struct KeyboardAction
{
    KeyboardActionType type;
};

struct FnKeysProfile
{
    QString name;
    QMap<FnKey, KeyboardAction> fnKeysMap;
};

struct ProfilesConfig
{
    QString currentPowerProfileName;
    FnKeysProfile fnKeysProfile;
    QList<FullPowerProfile> switchablePowerProfiles;
};

struct AppState
{
    PowerSource powerSource;
    G14ArmouryCratePlan arCratePlan;
    CpuState cpuState;
    FanCurve cpuFanCurve;
    FanCurve gpuFanCurve;
    KeyboardBklState kbBklState;
    bool isLidOpen = true;
    bool isMicEnabled = true;
    bool isTouchpadEnabled = true;
    int maxChargeLevel = 100;
    ProfilesConfig config;
};

#endif //G14APPSTATE_H
