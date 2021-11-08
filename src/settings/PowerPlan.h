#ifndef G14CONTROLPP_POWERPLAN_H
#define G14CONTROLPP_POWERPLAN_H

#include <QString>
#include "src/atkacpi/ArmouryCratePlan.h"
#include "src/atkacpi/FansProfile.h"
#include "src/ryzenadj/PowerProfile.h"

class PowerPlan {

public:
    PowerPlan(uchar armouryCratePlanId, const QString &fansProfileName, const QString &powerProfileName);

    PowerPlan();

    static PowerPlan fromQString(const QString &string);
    QString toQString();

    ArmouryCratePlan getArmouryCratePlan();
    FansProfile getFansProfile();
    PowerProfile getPowerProfile();

private:

    uchar armouryCratePlanId;
    QString fansProfileName;
    QString powerProfileName;
};

#endif //G14CONTROLPP_POWERPLAN_H
