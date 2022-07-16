#ifndef G14CONTROLPP_POWERPLAN_H
#define G14CONTROLPP_POWERPLAN_H

#include "ArmouryCratePlan.h"
#include "FansProfile.h"
#include "CpuProfile.h"

struct HwProfile {

public:
    HwProfile(const ArCrate::Plan &arCratePlan, const Fans::Profile &fansProfile, const Ryzen::Profile &cpuProfile)
        : arCratePlan(arCratePlan)
        , fansProfile(fansProfile)
        , cpuProfile(cpuProfile) { }

    ArCrate::Plan arCratePlan;
    Fans::Profile fansProfile;
    Ryzen::Profile cpuProfile;

    static const HwProfile Default;
};


#endif //G14CONTROLPP_POWERPLAN_H
