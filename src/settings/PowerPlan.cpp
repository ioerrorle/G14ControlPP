#include "PowerPlan.h"
#include "src/settings/Settings.h"

PowerPlan::PowerPlan(uchar armouryCratePlanId, const QString &fansProfileName, const QString &powerProfileName)
        : armouryCratePlanId(armouryCratePlanId), fansProfileName(fansProfileName),
          powerProfileName(powerProfileName) {

}

const ArmouryCratePlan PowerPlan::getArmouryCratePlan() const {
    return ArmouryCratePlan::plans()[armouryCratePlanId];
}

const FansProfile PowerPlan::getFansProfile() const {
    return Settings::getInstance().getFansProfile(fansProfileName);
}

const CpuProfile PowerPlan::getPowerProfile() const {
    return Settings::getInstance().getPowerProfile(powerProfileName);
}

PowerPlan PowerPlan::fromQString(const QString &string) {
    QStringList list = string.split(",");
    return PowerPlan(list[0].toInt(), list[1], list[2]);
}

QString PowerPlan::toQString() {
    return QString::asprintf("%d,%s,%s", armouryCratePlanId, fansProfileName.toLocal8Bit().data(), powerProfileName.toLocal8Bit().data());
}

PowerPlan::PowerPlan() {

}
