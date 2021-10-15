#include "PowerPlanSet.h"

bool operator<(const PowerPlanSet &lhs, const PowerPlanSet &rhs) {
    if (lhs.name < rhs.name)
        return true;
    if (rhs.name < lhs.name)
        return false;
    if (lhs.dcPowerPlan < rhs.dcPowerPlan)
        return true;
    if (rhs.dcPowerPlan < lhs.dcPowerPlan)
        return false;
    if (lhs.acPowerPlan < rhs.acPowerPlan)
        return true;
    if (rhs.acPowerPlan < lhs.acPowerPlan)
        return false;
    return lhs.usbPowerPlan < rhs.usbPowerPlan;
}

bool operator==(const PowerPlanSet &lhs, const PowerPlanSet &rhs) {
    return lhs.name == rhs.name &&
            lhs.dcPowerPlan == rhs.dcPowerPlan &&
            lhs.acPowerPlan == rhs.acPowerPlan &&
            lhs.usbPowerPlan == rhs.usbPowerPlan;
}

QDataStream &operator<<(QDataStream &os, const PowerPlanSet &set) {
    os << set.name;
    os << set.dcPowerPlan;
    os << set.acPowerPlan;
    os << set.usbPowerPlan;
    return os;
}

QDataStream &operator>>(QDataStream &is, PowerPlanSet & get) {
    is >> get.name;
    is >> get.dcPowerPlan;
    is >> get.acPowerPlan;
    is >> get.usbPowerPlan;
    return is;
}
