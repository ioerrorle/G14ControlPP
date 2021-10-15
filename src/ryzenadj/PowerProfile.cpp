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
    if (l.name < r.name) return true;
    if (r.name < l.name) return false;
    if (l.mode < r.mode) return true;
    if (r.mode < l.mode) return false;
    if (l.stapmLimit < r.stapmLimit) return true;
    if (r.stapmLimit < l.stapmLimit) return false;
    if (l.stapmTime < r.stapmTime) return true;
    if (r.stapmTime < l.stapmTime) return false;
    if (l.slowTime < r.slowTime) return true;
    if (r.slowTime < l.slowTime) return false;
    if (l.slowLimit < r.slowLimit) return true;
    if (r.slowLimit < l.slowLimit) return false;
    return l.fastLimit < r.fastLimit;
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