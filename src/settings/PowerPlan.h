#ifndef G14CONTROLPP_POWERPLAN_H
#define G14CONTROLPP_POWERPLAN_H

#include <QString>
#include "src/model/ArmouryCratePlan.h"
#include "src/model/FansProfile.h"
#include "src/model/CpuProfile.h"

class HwProfile {

public:
    HwProfile(uchar armouryCratePlanId, const QString &fansProfileName, const QString &powerProfileName);

    HwProfile();

    static HwProfile fromQString(const QString &string);
    QString toQString();

    const ArmouryCratePlan getArmouryCratePlan() const;
    const FansProfile getFansProfile() const;
    const CpuProfile getPowerProfile() const;

private:

    uchar armouryCratePlanId;
    QString fansProfileName;
    QString powerProfileName;
};

#endif //G14CONTROLPP_POWERPLAN_H
