#include "PowerProfile.h"

bool operator==(const PowerProfile &l, const PowerProfile &r) {
    return l.name == r.name
           && l.mode == r.mode
           && l.stapmTime == r.stapmTime
           && l.stapmLimit == r.stapmLimit
           && l.slowTime == r.slowTime
           && l.slowLimit == r.slowLimit
           && l.fastLimit == r.fastLimit;
}

bool operator<(const PowerProfile &l, const PowerProfile &r) {
    return strcmp(l.name.toLocal8Bit().constData(), r.name.toLocal8Bit().constData()) < 0
           || l.mode < r.mode
           || l.stapmTime < r.stapmTime
           || l.stapmLimit < r.stapmLimit
           || l.slowTime < r.slowTime
           || l.slowLimit < r.slowLimit
           || l.fastLimit < r.fastLimit;
}

QDataStream &operator<<(QDataStream &out, const PowerProfile &v) {
    out << v.name << v.slowLimit << v.slowTime << v.stapmLimit << v.stapmTime << v.fastLimit << v.mode;
    return out;
}

QDataStream &operator>>(QDataStream &in, PowerProfile &v) {
    in >> v.name;
    in >> v.slowLimit;
    in >> v.slowTime;
    in >> v.stapmLimit;
    in >> v.stapmTime;
    in >> v.fastLimit;
    in >> v.mode;
    return in;
}