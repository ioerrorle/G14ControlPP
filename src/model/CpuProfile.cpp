#include "CpuProfile.h"

CpuProfile::CpuProfile(const QString &name, float stapmLimit, float stapmTime, float slowLimit, float slowTime,
                       float fastLimit, Setpoint mode) : name(name), stapmLimit(stapmLimit), stapmTime(stapmTime),
                                                             slowLimit(slowLimit), slowTime(slowTime),
                                                             fastLimit(fastLimit), mode(mode) {

}

const QString &CpuProfile::getName() const {
    return name;
}

float CpuProfile::getStapmLimit() const {
    return stapmLimit;
}

float CpuProfile::getStapmTime() const {
    return stapmTime;
}

float CpuProfile::getSlowLimit() const {
    return slowLimit;
}

float CpuProfile::getSlowTime() const {
    return slowTime;
}

float CpuProfile::getFastLimit() const {
    return fastLimit;
}

Setpoint CpuProfile::getMode() const {
    return mode;
}

CpuProfile CpuProfile::fromQStringList(const QString &name, const QStringList &list) {
    return CpuProfile(name, list[0].toFloat(), list[1].toFloat(), list[2].toFloat(), list[3].toFloat(), list[4].toFloat(), list[5].toInt());
}

QStringList CpuProfile::toQStringList() const {
    auto result = QStringList();
    result.append(QString::number(stapmLimit));
    result.append(QString::number(stapmTime));
    result.append(QString::number(slowLimit));
    result.append(QString::number(slowTime));
    result.append(QString::number(fastLimit));
    result.append(QString::number(mode));
    return result;
}

CpuProfile CpuProfile::Default() {
    return CpuProfile(QString(), 0, 0, 0, 0, 0, 0);
}

bool CpuProfile::isEmpty() const {
    return empty;
}

CpuProfile::CpuProfile() {
    empty = true;
}

CpuProfile::CpuProfile(float stapmLimit, float stapmTime, float slowLimit, float slowTime, float fastLimit,
                       Setpoint mode) : stapmLimit(stapmLimit), stapmTime(stapmTime), slowLimit(slowLimit),
                                                    slowTime(slowTime), fastLimit(fastLimit), mode(mode) {}

void CpuProfile::setName(const QString &name) {
    CpuProfile::name = name;
}
