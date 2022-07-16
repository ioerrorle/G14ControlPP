#ifndef G14CONTROLPP_POWERPLAN_H
#define G14CONTROLPP_POWERPLAN_H

#include "ArmouryCratePlan.h"
#include "FansProfile.h"
#include "CpuProfile.h"

class HwProfile {

public:
    HwProfile(const ArCrate::Plan &armouryCratePlan, const FansProfile &fansProfile, const Ryzen::Profile &cpuProfile);

private:
    ArCrate::Plan armouryCratePlan;
    FansProfile fansProfile;
    Ryzen::Profile cpuProfile;
public:
    const ArCrate::Plan &getArmouryCratePlan() const;

    const FansProfile &getFansProfile() const;

    const Ryzen::Profile &getCpuProfile() const;

    static const HwProfile Default;
};


#endif //G14CONTROLPP_POWERPLAN_H
