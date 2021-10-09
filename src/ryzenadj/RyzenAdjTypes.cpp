#include "RyzenAdjTypes.h"

QDataStream &operator<<(QDataStream &out, const _PowerProfile &v) {
    out << v.slowLimit << v.slowTime << v.stapmLimit << v.stapmTime << v.fastLimit << v.mode;
    return out;
}

QDataStream &operator>>(QDataStream &in, _PowerProfile &v) {
    in >> v.slowLimit;
    in >> v.slowTime;
    in >> v.stapmLimit;
    in >> v.stapmTime;
    in >> v.fastLimit;
    in >> v.mode;
    return in;
}
