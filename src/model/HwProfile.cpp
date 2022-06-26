#include "HwProfile.h"

HwProfile::HwProfile(const ArmouryCratePlan &armouryCratePlan, const FansProfile &fansProfile,
                     const CpuProfile &cpuProfile) : armouryCratePlan(armouryCratePlan), fansProfile(fansProfile),
                                                     cpuProfile(cpuProfile) {

}

const ArmouryCratePlan &HwProfile::getArmouryCratePlan() const {
    return armouryCratePlan;
}

const FansProfile &HwProfile::getFansProfile() const {
    return fansProfile;
}

const CpuProfile &HwProfile::getCpuProfile() const {
    return cpuProfile;
}

const HwProfile HwProfile::Default = HwProfile(ArmouryCratePlan::Plans[0], FansProfile::Default, CpuProfile::Default);
