#include "PowerProfile.h"

PowerProfile::PowerProfile(const QString &name, float stapmLimit, float stapmTime, float slowLimit, float slowTime,
                           float fastLimit, Setpoint mode) : name(name), stapmLimit(stapmLimit), stapmTime(stapmTime),
                                                             slowLimit(slowLimit), slowTime(slowTime),
                                                             fastLimit(fastLimit), mode(mode) {

}

const QString &PowerProfile::getName() const {
    return name;
}

float PowerProfile::getStapmLimit() const {
    return stapmLimit;
}

float PowerProfile::getStapmTime() const {
    return stapmTime;
}

float PowerProfile::getSlowLimit() const {
    return slowLimit;
}

float PowerProfile::getSlowTime() const {
    return slowTime;
}

float PowerProfile::getFastLimit() const {
    return fastLimit;
}

Setpoint PowerProfile::getMode() const {
    return mode;
}

PowerProfile PowerProfile::fromQStringList(const QString &name, const QStringList &list) {
    return PowerProfile(name, list[0].toFloat(), list[1].toFloat(), list[2].toFloat(), list[3].toFloat(), list[4].toFloat(), list[5].toInt());
}

QStringList PowerProfile::toQStringList() {
    auto result = QStringList();
    result.append(QString::number(stapmLimit));
    result.append(QString::number(stapmTime));
    result.append(QString::number(slowLimit));
    result.append(QString::number(slowTime));
    result.append(QString::number(fastLimit));
    result.append(QString::number(mode));
    return result;
}

PowerProfile PowerProfile::Default() {
    return PowerProfile(QString(), 0, 0, 0, 0, 0, 0);
}

bool PowerProfile::isEmpty() {
    return empty;
}

PowerProfile::PowerProfile() {
    empty = true;
}
