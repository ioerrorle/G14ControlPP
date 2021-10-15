#include "PowerPlan.h"

QDataStream &operator<<(QDataStream &out, const PowerPlan &v) {
    out << v.armouryCratePlanId;
    out << v.fansProfile;
    out << v.powerProfile;
    return out;
}

QDataStream &operator>>(QDataStream &in, PowerPlan &v) {
    in >> v.armouryCratePlanId;
    in >> v.fansProfile;
    in >> v.powerProfile;
    return in;
}

bool operator==(const PowerPlan& lhs, const PowerPlan& rhs){
    return lhs.powerProfile == rhs.powerProfile
           && lhs.fansProfile == rhs.fansProfile
           && lhs.armouryCratePlanId == rhs.armouryCratePlanId;
}

bool operator<(const PowerPlan &l, const PowerPlan &r) {
    if (l.powerProfile < r.powerProfile) return true;
    if (r.powerProfile < l.powerProfile) return false;
    if (l.fansProfile < r.fansProfile) return true;
    if (r.fansProfile < l.fansProfile) return false;
    return l.armouryCratePlanId < r.armouryCratePlanId;
}
