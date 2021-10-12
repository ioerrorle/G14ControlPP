#ifndef G14CONTROLPP_POWERPROFILE_H
#define G14CONTROLPP_POWERPROFILE_H

#include <QDataStream>
#include <QString>

typedef uchar Setpoint;
static Setpoint SP_PERFORMANCE = 0;
static Setpoint SP_POWER_SAVING = 1;
static Setpoint SP_DEFAULT = 2;

typedef struct {
    QString name;
    float stapmLimit;
    float stapmTime;
    float slowLimit;
    float slowTime;
    float fastLimit;
    Setpoint mode;
} PowerProfile;

static PowerProfile STOCK_PROFILE {
        "Stock", 35, 200, 42, 5, 60, SP_DEFAULT
};

bool operator==(const PowerProfile& l, const PowerProfile& r);
bool operator<(const PowerProfile& l, const PowerProfile& r);
QDataStream &operator<<(QDataStream &out, const PowerProfile &v);
QDataStream &operator>>(QDataStream &in, PowerProfile &v);

Q_DECLARE_METATYPE(PowerProfile);

#endif //G14CONTROLPP_POWERPROFILE_H
