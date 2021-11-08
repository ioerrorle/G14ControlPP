#ifndef G14CONTROLPP_ARMOURYCRATEPLAN_H
#define G14CONTROLPP_ARMOURYCRATEPLAN_H

#include <QMap>

typedef uchar ASUS_PLAN;
typedef uchar FAN_DEVICE;

#define PLAN_PerformanceWindows 0x00
#define PLAN_TurboManual 0x01
#define PLAN_Silent 0x02

class ArmouryCratePlan {

public:
    static QMap<uchar, ArmouryCratePlan> plans();

    uint getId() const;

    const QString &getName() const;

    ASUS_PLAN getAsusPlanCode() const;

private:
    uint id;
    QString name;
    ASUS_PLAN asusPlanCode;
    ArmouryCratePlan(uint id, const QString &name, ASUS_PLAN asusPlanCode);
};


#endif //G14CONTROLPP_ARMOURYCRATEPLAN_H
