#ifndef G14CONTROLPP_FULLPOWERPLAN_H
#define G14CONTROLPP_FULLPOWERPLAN_H

#include "HwProfile.h"

struct PowerProfile {
    Q_DECLARE_TR_FUNCTIONS(PowerProfile);
public:
    QString name;
    HwProfile dc;
    HwProfile usb;
    HwProfile ac;

    PowerProfile(const QString &name, const HwProfile &dc, const HwProfile &usb, const HwProfile &ac)
        : name(name)
        , dc(dc)
        , usb(usb)
        , ac(ac) {}

    PowerProfile()
        : name(DefaultName)
        , dc(HwProfile::Default)
        , usb(HwProfile::Default)
        , ac(HwProfile::Default) {

    }
    const HwProfile &getHwProfile(uchar powerSourceId) const;

    static const PowerProfile Default;
    static const QString DefaultName;
};


#endif
