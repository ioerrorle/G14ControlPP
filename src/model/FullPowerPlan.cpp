#include "FullPowerPlan.h"

FullPowerPlan::FullPowerPlan(const QString &name, const PowerPlan &battery, const PowerPlan &typeC,
                             const PowerPlan &charger) : name(name), battery(battery), typeC(typeC), charger(charger) {

}

const QString &FullPowerPlan::getName() const {
    return name;
}

const PowerPlan &FullPowerPlan::getBattery() const {
    return battery;
}

const PowerPlan &FullPowerPlan::getTypeC() const {
    return typeC;
}

const PowerPlan &FullPowerPlan::getCharger() const {
    return charger;
}
