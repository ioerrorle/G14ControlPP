#include "PowerPlanSet.h"

//bool operator<(const PowerPlanSet &lhs, const PowerPlanSet &rhs) {
//    if (lhs.name < rhs.name)
//        return true;
//    if (rhs.name < lhs.name)
//        return false;
//    if (lhs.dcPowerPlan < rhs.dcPowerPlan)
//        return true;
//    if (rhs.dcPowerPlan < lhs.dcPowerPlan)
//        return false;
//    if (lhs.acPowerPlan < rhs.acPowerPlan)
//        return true;
//    if (rhs.acPowerPlan < lhs.acPowerPlan)
//        return false;
//    return lhs.usbPowerPlan < rhs.usbPowerPlan;
//}
//
//bool operator==(const PowerPlanSet &lhs, const PowerPlanSet &rhs) {
//    return lhs.name == rhs.name &&
//            lhs.dcPowerPlan == rhs.dcPowerPlan &&
//            lhs.acPowerPlan == rhs.acPowerPlan &&
//            lhs.usbPowerPlan == rhs.usbPowerPlan;
//}
//
//QDataStream &operator<<(QDataStream &os, const PowerPlanSet &set) {
//    os << set.name;
//    os << set.dcPowerPlan;
//    os << set.acPowerPlan;
//    os << set.usbPowerPlan;
//    return os;
//}
//
//QDataStream &operator>>(QDataStream &is, PowerPlanSet & get) {
//    is >> get.name;
//    is >> get.dcPowerPlan;
//    is >> get.acPowerPlan;
//    is >> get.usbPowerPlan;
//    return is;
//}

PowerPlanSet::PowerPlanSet(const QString &name, const PowerPlan &dcPowerPlan, const PowerPlan &acPowerPlan,
                           const PowerPlan &usbPowerPlan) : name(name), dcPowerPlan(dcPowerPlan),
                                                            acPowerPlan(acPowerPlan), usbPowerPlan(usbPowerPlan) {

}

const QString &PowerPlanSet::getName() const {
    return name;
}

const PowerPlan &PowerPlanSet::getDcPowerPlan() const {
    return dcPowerPlan;
}

const PowerPlan &PowerPlanSet::getAcPowerPlan() const {
    return acPowerPlan;
}

const PowerPlan &PowerPlanSet::getUsbPowerPlan() const {
    return usbPowerPlan;
}

PowerPlanSet PowerPlanSet::fromQStringList(const QString &name, const QStringList &data) {
    return PowerPlanSet(name, PowerPlan::fromQString(data[0]), PowerPlan::fromQString(data[1]), PowerPlan::fromQString(data[2]));
}

QStringList PowerPlanSet::toQStringList() {
    QStringList result;
    result.append(dcPowerPlan.toQString());
    result.append(acPowerPlan.toQString());
    result.append(usbPowerPlan.toQString());
    return result;
}

PowerPlanSet::PowerPlanSet() {
    empty = true;
}

bool PowerPlanSet::isEmpty() const {
    return empty;
}
