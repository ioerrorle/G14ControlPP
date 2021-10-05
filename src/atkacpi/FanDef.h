#ifndef G14CONTROLPP_FANDEF_H
#define G14CONTROLPP_FANDEF_H

#include <QDataStream>

typedef uchar ASUS_PLAN;
typedef uchar FAN_DEVICE;

#define PLAN_PerformanceWindows ASUS_PLAN(0x00)
#define PLAN_TurboManual ASUS_PLAN(0x01)
#define PLAN_Silent ASUS_PLAN(0x02)

#define FAN_CPU FAN_DEVICE(0x24)
#define FAN_GPU FAN_DEVICE(0x25)

struct _FanCurve {
    uchar temp[8];
    uchar speed[8];
};
typedef _FanCurve FanCurve;

Q_DECLARE_METATYPE(_FanCurve);

struct _FansProfile {
    QString name;
    _FanCurve cpu;
    _FanCurve gpu;
};

typedef _FansProfile FansProfile;

Q_DECLARE_METATYPE(_FansProfile);

#endif //G14CONTROLPP_FANDEF_H
