#ifndef G14CONTROLPP_POWERPLANSET_H
#define G14CONTROLPP_POWERPLANSET_H

#include <QString>
#include <QDataStream>
#include "PowerPlan.h"

class PowerPlanSet {
public:
    PowerPlanSet(const QString &name, const PowerPlan &dcPowerPlan, const PowerPlan &acPowerPlan,
                 const PowerPlan &usbPowerPlan);

    PowerPlanSet();

    static PowerPlanSet fromQStringList(const QString &name, const QStringList &data);

    QStringList toQStringList();

    const QString &getName() const;

    const PowerPlan &getDcPowerPlan() const;

    const PowerPlan &getAcPowerPlan() const;

    const PowerPlan &getUsbPowerPlan() const;

    bool isEmpty() const;

private:
    QString name;
    PowerPlan dcPowerPlan;
    PowerPlan acPowerPlan;
    PowerPlan usbPowerPlan;
    bool empty = false;
};
//typedef struct {
//    QString name;
//    PowerPlan dcPowerPlan;
//    PowerPlan acPowerPlan;
//    PowerPlan usbPowerPlan;
//} PowerPlanSet;
//
//bool operator<(const PowerPlanSet &lhs, const PowerPlanSet &rhs);
//bool operator==(const PowerPlanSet &lhs, const PowerPlanSet &rhs);
//QDataStream &operator<<(QDataStream &os, const PowerPlanSet &set);
//QDataStream &operator>>(QDataStream &is, PowerPlanSet &get);
//
//Q_DECLARE_METATYPE(PowerPlanSet);

#endif //G14CONTROLPP_POWERPLANSET_H
