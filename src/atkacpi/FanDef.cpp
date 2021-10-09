#include "FanDef.h"
QDataStream &operator<<(QDataStream &out, const _FanCurve &v) {
    for (int i = 0; i < 8; i++) {
        out << v.temp[i];
        out << v.speed[i];
    }
    return out;
}

QDataStream &operator>>(QDataStream &in, _FanCurve &v) {
    for (int i = 0; i < 8; i++) {
        in >> v.temp[i];
        in >> v.speed[i];
    }
    return in;
}

QDataStream &operator<<(QDataStream &out, const _FansProfile &v) {
    out << v.name;
    out << v.cpu;
    out << v.gpu;
    return out;
}

QDataStream &operator>>(QDataStream &in, _FansProfile &v) {
    in >> v.name;
    in >> v.cpu;
    in >> v.gpu;
    return in;
}