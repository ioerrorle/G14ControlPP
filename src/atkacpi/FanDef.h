#ifndef G14CONTROLPP_FANDEF_H
#define G14CONTROLPP_FANDEF_H

#include <QDataStream>
#include <QMap>

typedef uchar ASUS_PLAN;
typedef uchar FAN_DEVICE;

#define PLAN_PerformanceWindows 0x00
#define PLAN_TurboManual 0x01
#define PLAN_Silent 0x02

typedef struct {
    uint id;
    QString name;
    ASUS_PLAN asusPlanCode;
} ArmouryCratePlan;

static QMap<uchar, ArmouryCratePlan> ARMOURY_CRATE_PLANS{
        {0, {0, "Windows/Performance", PLAN_PerformanceWindows}},
        {1, {1, "Turbo/Manual", PLAN_TurboManual}},
        {2, {2, "Silent", PLAN_Silent}}
};

//inline bool operator==(const ArmouryCratePlan& l, const ArmouryCratePlan& r);

Q_DECLARE_METATYPE(ArmouryCratePlan);

#define FAN_CPU FAN_DEVICE(0x24)
#define FAN_GPU FAN_DEVICE(0x25)

typedef struct {
    uchar temp[8];
    uchar speed[8];
} FanCurve;

bool operator==(const FanCurve& l, const FanCurve& r);
bool operator<(const FanCurve& l, const FanCurve& r);
QDataStream &operator<<(QDataStream &out, const FanCurve &v);
QDataStream &operator>>(QDataStream &in, FanCurve &v);

Q_DECLARE_METATYPE(FanCurve);

typedef struct {
    QString name;
    FanCurve cpu;
    FanCurve gpu;
} FansProfile;

bool operator==(const FansProfile& l, const FansProfile& r);
bool operator<(const FansProfile& l, const FansProfile& r);
QDataStream &operator<<(QDataStream &out, const FansProfile &v);
QDataStream &operator>>(QDataStream &in, FansProfile &v);

Q_DECLARE_METATYPE(FansProfile);

#endif //G14CONTROLPP_FANDEF_H
