#ifndef G14CONTROLPP_FANDEF_H
#define G14CONTROLPP_FANDEF_H

#include <QDataStream>
#include <QMap>

typedef uchar ASUS_PLAN;
typedef uchar FAN_DEVICE;

#define PLAN_PerformanceWindows ASUS_PLAN(0x00)
#define PLAN_TurboManual ASUS_PLAN(0x01)
#define PLAN_Silent ASUS_PLAN(0x02)

struct _ArmouryCratePowerPlan {
    uint id;
    QString name;
    ASUS_PLAN powerPlan;
};

typedef _ArmouryCratePowerPlan ArmouryCratePowerPlan;

static QMap<uchar, ArmouryCratePowerPlan> POWER_PLANS{
        {0, {0, "Windows/Performance", PLAN_PerformanceWindows}},
        {1, {1, "Turbo/Manual", PLAN_TurboManual}},
        {2, {2, "Silent", PLAN_Silent}}
};

QDataStream &operator<<(QDataStream &out, const _ArmouryCratePowerPlan &v);
QDataStream &operator>>(QDataStream &in, _ArmouryCratePowerPlan &v);

Q_DECLARE_METATYPE(ArmouryCratePowerPlan);

#define FAN_CPU FAN_DEVICE(0x24)
#define FAN_GPU FAN_DEVICE(0x25)

struct _FanCurve {
    uchar temp[8];
    uchar speed[8];
};
typedef _FanCurve FanCurve;

QDataStream &operator<<(QDataStream &out, const _FanCurve &v);
QDataStream &operator>>(QDataStream &in, _FanCurve &v);

Q_DECLARE_METATYPE(_FanCurve);

struct _FansProfile {
    QString name;
    _FanCurve cpu;
    _FanCurve gpu;
};

typedef _FansProfile FansProfile;

QDataStream &operator<<(QDataStream &out, const _FansProfile &v);
QDataStream &operator>>(QDataStream &in, _FansProfile &v);

Q_DECLARE_METATYPE(_FansProfile);

#endif //G14CONTROLPP_FANDEF_H
