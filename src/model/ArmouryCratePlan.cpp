#include "ArmouryCratePlan.h"

ArmouryCratePlan::ArmouryCratePlan(ASUS_PLAN asusPlanCode, const QString &name)
    : id(asusPlanCode)
    , name(name) {}

const QList<ArmouryCratePlan> ArmouryCratePlan::Plans = QList<ArmouryCratePlan>({
    ArmouryCratePlan(PLAN_PerformanceWindows, "Windows/Performance"),
    ArmouryCratePlan(PLAN_TurboManual, "Turbo/Manual"),
    ArmouryCratePlan(PLAN_Silent, "Silent")});

ASUS_PLAN ArmouryCratePlan::getId() const {
    return id;
}

const QString &ArmouryCratePlan::getName() const {
    return name;
}

ArmouryCratePlan::ArmouryCratePlan() {}
