#ifndef G14CONTROLPP_POWERPLAN_H
#define G14CONTROLPP_POWERPLAN_H

#include "ArmouryCratePlan.h"
#include "FansProfile.h"
#include "CpuProfile.h"

class HwProfile {

public:
    HwProfile(const ArmouryCratePlan &armouryCratePlan, const FansProfile &fansProfile, const CpuProfile &cpuProfile);

private:
    ArmouryCratePlan armouryCratePlan;
    FansProfile fansProfile;
    CpuProfile cpuProfile;
public:
    const ArmouryCratePlan &getArmouryCratePlan() const;

    const FansProfile &getFansProfile() const;

    const CpuProfile &getCpuProfile() const;

    static const HwProfile Default;
};


#endif //G14CONTROLPP_POWERPLAN_H
