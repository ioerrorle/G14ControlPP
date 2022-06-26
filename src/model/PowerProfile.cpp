#include "PowerProfile.h"

PowerProfile::PowerProfile(const QString &name,
                           const HwProfile &dc,
                           const HwProfile &usb,
                           const HwProfile &ac)
    : name(name)
    , dc(dc)
    , usb(usb)
    , ac(ac) {}

PowerProfile::PowerProfile()
    : name("<Default>")
    , dc(HwProfile::Default)
    , usb(HwProfile::Default)
    , ac(HwProfile::Default) {}

const QString &PowerProfile::getName() const {
    return name;
}

const HwProfile &PowerProfile::getDc() const {
    return dc;
}

const HwProfile &PowerProfile::getUsb() const {
    return usb;
}

const HwProfile &PowerProfile::getAc() const {
    return ac;
}

const HwProfile &PowerProfile::getHwProfile(uchar powerSourceId) const {
    switch (powerSourceId) {
    case 0:
        return dc;
    case 1:
        return usb;
    default:
        return ac;
    }
}

const PowerProfile PowerProfile::Default = PowerProfile();
