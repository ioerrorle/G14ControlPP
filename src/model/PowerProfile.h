#ifndef G14CONTROLPP_FULLPOWERPLAN_H
#define G14CONTROLPP_FULLPOWERPLAN_H

#include "HwProfile.h"

class PowerProfile {

public:
    PowerProfile(const QString &name, const HwProfile &dc, const HwProfile &usb, const HwProfile &ac);

    PowerProfile();

    const QString &getName() const;

    const HwProfile &getDc() const;

    const HwProfile &getUsb() const;

    const HwProfile &getAc() const;

    const HwProfile &getHwProfile(uchar powerSourceId) const;

    static const PowerProfile Default;

private:
    QString name;
    HwProfile dc;
    HwProfile usb;
    HwProfile ac;
};


#endif
