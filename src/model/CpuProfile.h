#ifndef G14CONTROLPP_CPUPROFILE_H
#define G14CONTROLPP_CPUPROFILE_H

#include <QStringList>
#include <QMetaType>

enum CpuMode
{
    SP_PERFORMANCE = 0,
    SP_POWER_SAVING = 1,
    SP_DEFAULT = 2
};

struct NamedCpuMode {
    CpuMode setPoint;
    QString name;
};

enum CpuProfileType {
    CPU_PROFILE_DEFAULT = 0,
    CPU_PROFILE_CURRENT = 1,
    CPU_PROFILE_USER_DEFINED = 2
};

class CpuProfile {
public:
    CpuProfile(const QString &name,
               float stapmLimit,
               float stapmTime,
               float slowLimit,
               float slowTime,
               float fastLimit,
               CpuMode mode);

    CpuProfile(CpuProfileType type = CPU_PROFILE_DEFAULT);

    const QString &name() const;
    float stapmLimit() const;
    float stapmTime() const;
    float slowLimit() const;
    float slowTime() const;
    float fastLimit() const;
    CpuMode mode() const;
    CpuProfileType type() const;

    static const CpuProfile Default;
    static const CpuProfile Current;

    static const QList<NamedCpuMode> CpuModes;

    static CpuProfile fromQStringList(const QString &name, const QStringList &list);
    QStringList toQStringList() const;

private:
    QString m_name;
    float m_stapmLimit;
    float m_stapmTime;
    float m_slowLimit;
    float m_slowTime;
    float m_fastLimit;
    CpuMode m_mode;
    CpuProfileType m_type = CPU_PROFILE_DEFAULT;

    static const QString DefaultName;
    static const QString CurrentName;
};

Q_DECLARE_METATYPE(CpuProfile)

#endif //G14CONTROLPP_CPUPROFILE_H
