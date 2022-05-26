#include "PowerPlan.h"

PowerPlan::PowerPlan(const ArmouryCratePlan &armouryCratePlan, const FansProfile &fansProfile,
                     const CpuProfile &cpuProfile) : armouryCratePlan(armouryCratePlan), fansProfile(fansProfile),
                                                     cpuProfile(cpuProfile) {

}

const ArmouryCratePlan &PowerPlan::getArmouryCratePlan() const {
    return armouryCratePlan;
}

const FansProfile &PowerPlan::getFansProfile() const {
    return fansProfile;
}

const CpuProfile &PowerPlan::getCpuProfile() const {
    return cpuProfile;
}
