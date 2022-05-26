#ifndef G14CONTROLPP_CPUPROFILE_H
#define G14CONTROLPP_CPUPROFILE_H

#include <QStringList>

typedef uchar Setpoint;
static Setpoint SP_PERFORMANCE = 0;
static Setpoint SP_POWER_SAVING = 1;
static Setpoint SP_DEFAULT = 2;

class CpuProfile {
public:
    static CpuProfile Default();
    static CpuProfile fromQStringList(const QString &name, const QStringList &list);

public:
    CpuProfile(const QString &name, float stapmLimit, float stapmTime, float slowLimit, float slowTime,
               float fastLimit, Setpoint mode);

    CpuProfile();

    void setName(const QString &name);

    CpuProfile(float stapmLimit, float stapmTime, float slowLimit, float slowTime, float fastLimit, Setpoint mode);

    const QString &getName() const;

    float getStapmLimit() const;

    float getStapmTime() const;

    float getSlowLimit() const;

    float getSlowTime() const;

    float getFastLimit() const;

    Setpoint getMode() const;

    QStringList toQStringList() const;

    bool isEmpty() const;

private:
    QString name;
    float stapmLimit;
    float stapmTime;
    float slowLimit;
    float slowTime;
    float fastLimit;
    Setpoint mode;
    bool empty = false;
};

#endif //G14CONTROLPP_CPUPROFILE_H
