#include "PowerPlan.h"
#include "src/settings/Settings.h"

HwProfile::HwProfile(uchar armouryCratePlanId, const QString &fansProfileName, const QString &powerProfileName)
        : armouryCratePlanId(armouryCratePlanId), fansProfileName(fansProfileName),
          powerProfileName(powerProfileName) {

}

const ArmouryCratePlan HwProfile::getArmouryCratePlan() const {
    return ArmouryCratePlan::plans()[armouryCratePlanId];
}

const FansProfile HwProfile::getFansProfile() const {
    return Settings::getInstance().getFansProfile(fansProfileName);
}

const CpuProfile HwProfile::getPowerProfile() const {
    return Settings::getInstance().getPowerProfile(powerProfileName);
}

HwProfile HwProfile::fromQString(const QString &string) {
    QStringList list = string.split(",");
    return HwProfile(list[0].toInt(), list[1], list[2]);
}

QString HwProfile::toQString() {
    return QString::asprintf("%d,%s,%s", armouryCratePlanId, fansProfileName.toLocal8Bit().data(), powerProfileName.toLocal8Bit().data());
}

HwProfile::HwProfile() {

}
