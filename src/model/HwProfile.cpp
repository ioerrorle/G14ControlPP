#include "HwProfile.h"

HwProfile::HwProfile(const ArCrate::Plan &armouryCratePlan, const FansProfile &fansProfile,
                     const Ryzen::Profile &cpuProfile) : armouryCratePlan(armouryCratePlan), fansProfile(fansProfile),
                                                     cpuProfile(cpuProfile) {

}

const ArCrate::Plan &HwProfile::getArmouryCratePlan() const {
    return armouryCratePlan;
}

const FansProfile &HwProfile::getFansProfile() const {
    return fansProfile;
}

const Ryzen::Profile &HwProfile::getCpuProfile() const {
    return cpuProfile;
}

const HwProfile HwProfile::Default = HwProfile(ArCrate::Plan(), FansProfile::Default, Ryzen::Profile::Default);
