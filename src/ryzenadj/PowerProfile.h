#ifndef G14CONTROLPP_POWERPROFILE_H
#define G14CONTROLPP_POWERPROFILE_H

#include <QStringList>

typedef uchar Setpoint;
static Setpoint SP_PERFORMANCE = 0;
static Setpoint SP_POWER_SAVING = 1;
static Setpoint SP_DEFAULT = 2;

class PowerProfile {
public:
    static PowerProfile Default();
    static PowerProfile fromQStringList(const QString &name, const QStringList &list);

public:
    PowerProfile(const QString &name, float stapmLimit, float stapmTime, float slowLimit, float slowTime,
                 float fastLimit, Setpoint mode);

    PowerProfile();

    const QString &getName() const;

    float getStapmLimit() const;

    float getStapmTime() const;

    float getSlowLimit() const;

    float getSlowTime() const;

    float getFastLimit() const;

    Setpoint getMode() const;

    QStringList toQStringList();

    bool isEmpty();

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

#endif //G14CONTROLPP_POWERPROFILE_H
