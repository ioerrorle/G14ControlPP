#ifndef G14CONTROLPP_FULLPOWERPLAN_H
#define G14CONTROLPP_FULLPOWERPLAN_H

#include "PowerPlan.h"

class FullPowerPlan {

public:
    FullPowerPlan(const QString &name, const PowerPlan &battery, const PowerPlan &typeC, const PowerPlan &charger);

    const QString &getName() const;

    const PowerPlan &getBattery() const;

    const PowerPlan &getTypeC() const;

    const PowerPlan &getCharger() const;

private:
    QString name;
    PowerPlan battery;
    PowerPlan typeC;
    PowerPlan charger;
};


#endif
