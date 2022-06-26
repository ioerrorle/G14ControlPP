#ifndef G14CONTROLPP_ARMOURYCRATEPLAN_H
#define G14CONTROLPP_ARMOURYCRATEPLAN_H

#include <QString>
#include <QList>

typedef uchar ASUS_PLAN;
typedef uchar FAN_DEVICE;

enum ArmouryCratePlanCode {
    PLAN_PerformanceWindows = 0x00,
    PLAN_TurboManual = 0x01,
    PLAN_Silent = 0x02
};

class ArmouryCratePlan {

public:
    ArmouryCratePlan();

    ASUS_PLAN getId() const;

    const QString &getName() const;

    static const QList<ArmouryCratePlan> Plans;

private:
    ASUS_PLAN id;
    QString name;
    ArmouryCratePlan(ASUS_PLAN asusPlanCode, const QString &name);
};


#endif //G14CONTROLPP_ARMOURYCRATEPLAN_H
