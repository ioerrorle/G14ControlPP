#ifndef G14CONTROLPP_POWERPLAN_H
#define G14CONTROLPP_POWERPLAN_H

#include "ArmouryCratePlan.h"
#include "FansProfile.h"
#include "CpuProfile.h"

class PowerPlan {

public:
    PowerPlan(const ArmouryCratePlan &armouryCratePlan, const FansProfile &fansProfile, const CpuProfile &cpuProfile);

private:
    ArmouryCratePlan armouryCratePlan;
    FansProfile fansProfile;
    CpuProfile cpuProfile;
public:
    const ArmouryCratePlan &getArmouryCratePlan() const;

    const FansProfile &getFansProfile() const;

    const CpuProfile &getCpuProfile() const;
};


#endif //G14CONTROLPP_POWERPLAN_H
