#include "ArmouryCratePlan.h"

ArmouryCratePlan::ArmouryCratePlan(uint id, const QString &name, ASUS_PLAN asusPlanCode) : id(id), name(name),
                                                                                           asusPlanCode(asusPlanCode) {

}

QMap<uchar, ArmouryCratePlan> ArmouryCratePlan::plans() {
    static QMap<uchar, ArmouryCratePlan> plansMapInstance{
            {0, ArmouryCratePlan(0, "Windows/Performance", PLAN_PerformanceWindows)},
            {1, ArmouryCratePlan(1, "Turbo/Manual", PLAN_TurboManual)},
            {2, ArmouryCratePlan(2, "Silent", PLAN_Silent)}
    };
    return plansMapInstance;
}

uint ArmouryCratePlan::getId() const {
    return id;
}

const QString &ArmouryCratePlan::getName() const {
    return name;
}

ASUS_PLAN ArmouryCratePlan::getAsusPlanCode() const {
    return asusPlanCode;
}

ArmouryCratePlan::ArmouryCratePlan() {

}
