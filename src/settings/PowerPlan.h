#ifndef G14CONTROLPP_POWERPLAN_H
#define G14CONTROLPP_POWERPLAN_H

#include <QDataStream>
#include "src/atkacpi/FanDef.h"
#include "src/ryzenadj/PowerProfile.h"

typedef struct {
    uchar armouryCratePlanId;
    FansProfile fansProfile;
    PowerProfile powerProfile;

    ArmouryCratePlan armouryCratePlan() { return ARMOURY_CRATE_PLANS[armouryCratePlanId]; };
} PowerPlan;

bool operator==(const PowerPlan &lhs, const PowerPlan &rhs);

bool operator<(const PowerPlan &l, const PowerPlan &r);

QDataStream &operator<<(QDataStream &out, const PowerPlan &v);

QDataStream &operator>>(QDataStream &in, PowerPlan &v);

Q_DECLARE_METATYPE(PowerPlan);

#endif //G14CONTROLPP_POWERPLAN_H
