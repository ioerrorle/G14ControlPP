#ifndef G14CONTROLPP_POWERPLANSET_H
#define G14CONTROLPP_POWERPLANSET_H

#include <QString>
#include <QDataStream>
#include "PowerPlan.h"

typedef struct {
    QString name;
    PowerPlan dcPowerPlan;
    PowerPlan acPowerPlan;
    PowerPlan usbPowerPlan;
} PowerPlanSet;

bool operator<(const PowerPlanSet &lhs, const PowerPlanSet &rhs);
bool operator==(const PowerPlanSet &lhs, const PowerPlanSet &rhs);
QDataStream &operator<<(QDataStream &os, const PowerPlanSet &set);
QDataStream &operator>>(QDataStream &is, PowerPlanSet &get);

Q_DECLARE_METATYPE(PowerPlanSet);

#endif //G14CONTROLPP_POWERPLANSET_H
