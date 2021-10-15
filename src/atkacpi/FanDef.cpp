#include "FanDef.h"

QDataStream &operator<<(QDataStream &out, const FanCurve &v) {
    for (int i = 0; i < 8; i++) {
        out << v.temp[i];
        out << v.speed[i];
    }
    return out;
}

QDataStream &operator>>(QDataStream &in, FanCurve &v) {
    for (int i = 0; i < 8; i++) {
        in >> v.temp[i];
        in >> v.speed[i];
    }
    return in;
}

QDataStream &operator<<(QDataStream &out, const FansProfile &v) {
    out << v.name;
    out << v.cpu;
    out << v.gpu;
    return out;
}

QDataStream &operator>>(QDataStream &in, FansProfile &v) {
    in >> v.name;
    in >> v.cpu;
    in >> v.gpu;
    return in;
}

bool operator<(const FanCurve &l, const FanCurve &r) {
    for (int i = 0; i < 8; i++) {
        if (l.speed[i] < r.speed[i])
            return true;
        if (r.speed[i] < l.speed[i])
            return false;
        if (l.temp[i] < r.temp[i])
            return true;
        if (r.temp[i] < l.temp[i])
            return false;
    }
    return false;
}

bool operator==(const FanCurve &l, const FanCurve &r) {
    for (int i = 0; i < 8; i++) {
        if (l.speed[i] != r.speed[i]
            || l.temp[i] != r.temp[i])
            return false;
    }
    return true;
}

bool operator<(const FansProfile &l, const FansProfile &r) {
    if (l.name < r.name)
        return true;
    if (r.name < l.name)
        return false;
    if (l.cpu < r.cpu)
        return true;
    if (r.cpu < l.cpu)
        return false;
    return l.gpu < r.gpu;
}

bool operator==(const FansProfile &l, const FansProfile &r) {
    return l.cpu == r.cpu && l.name == r.name && l.gpu == r.gpu;
}
