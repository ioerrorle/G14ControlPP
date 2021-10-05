#include "FanDef.h"

QDataStream &operator<<(QDataStream &out, const _FanCurve &v) {
    out.writeRawData((const char *) &v.temp[0], 8);
    out.writeRawData((const char *) &v.speed[0], 8);

    return out;
}

QDataStream &operator>>(QDataStream &in, _FanCurve &v) {
    //for (int i = 0; i < 8; i++) {
    uint len = 8;
    char *tmpTemp;
    char *tmpSpeed;
    in.readBytes(tmpTemp, len);
    len = 8;
    in.readBytes(tmpSpeed, len);

    for (int i = 0; i < len; i++) {
        v.temp[i] = tmpTemp[i];
        v.speed[i] = tmpSpeed[i];
    }
    //}
    return in;
}

QDataStream &operator<<(QDataStream &out, const _FansProfile &v) {
    out << v.name << v.cpu << v.gpu;
    return out;
}

QDataStream &operator>>(QDataStream &in, _FansProfile &v) {
    in >> v.name;
    in >> v.cpu;
    in >> v.gpu;
    return in;
}