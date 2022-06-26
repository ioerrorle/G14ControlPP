#include "CpuProfile.h"

CpuProfile::CpuProfile(const QString &name,
                       float stapmLimit,
                       float stapmTime,
                       float slowLimit,
                       float slowTime,
                       float fastLimit,
                       CpuMode mode)
    : m_name(name)
    , m_stapmLimit(stapmLimit)
    , m_stapmTime(stapmTime)
    , m_slowLimit(slowLimit)
    , m_slowTime(slowTime)
    , m_fastLimit(fastLimit)
    , m_mode(mode)
    , m_type(CPU_PROFILE_USER_DEFINED) {}

CpuProfile::CpuProfile(CpuProfileType type) : m_type(type)
{

}

const QString &CpuProfile::name() const {
    if (m_type == CPU_PROFILE_DEFAULT)
        return DefaultName;
    if (m_type == CPU_PROFILE_CURRENT)
        return CurrentName;
    return m_name;
}

float CpuProfile::stapmLimit() const {
    return m_stapmLimit;
}

float CpuProfile::stapmTime() const {
    return m_stapmTime;
}

float CpuProfile::slowLimit() const {
    return m_slowLimit;
}

float CpuProfile::slowTime() const {
    return m_slowTime;
}

float CpuProfile::fastLimit() const {
    return m_fastLimit;
}

CpuMode CpuProfile::mode() const {
    return m_mode;
}

CpuProfile CpuProfile::fromQStringList(const QString &name, const QStringList &list) {
    if (list.size() == 0 || list[0] == "default")
        return Default;

    return CpuProfile(name,
                      list[0].toFloat(),
            list[1].toFloat(),
            list[2].toFloat(),
            list[3].toFloat(),
            list[4].toFloat(),
            static_cast<CpuMode>(list[5].toInt()));
}

QStringList CpuProfile::toQStringList() const {
    auto result = QStringList();
    if (m_type == CPU_PROFILE_DEFAULT) {
        result.append("default");
    } else {
        result.append(QString::number(m_stapmLimit));
        result.append(QString::number(m_stapmTime));
        result.append(QString::number(m_slowLimit));
        result.append(QString::number(m_slowTime));
        result.append(QString::number(m_fastLimit));
        result.append(QString::number(m_mode));
    }
    return result;
}

CpuProfileType CpuProfile::type() const
{
    return m_type;
}

const CpuProfile CpuProfile::Default = CpuProfile();
const CpuProfile CpuProfile::Current = CpuProfile(CPU_PROFILE_CURRENT);

const QString CpuProfile::DefaultName = "<Default>";
const QString CpuProfile::CurrentName = "<Current>";

const QList<NamedCpuMode> CpuProfile::CpuModes =
        QList<NamedCpuMode>({
                                 {SP_PERFORMANCE, "Performance"},
                                 {SP_POWER_SAVING, "Power saving"},
                                 {SP_DEFAULT, "Default"}
                             });
