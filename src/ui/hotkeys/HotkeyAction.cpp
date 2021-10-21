#include "HotkeyAction.h"

bool operator==(const HotkeyAction &l, const HotkeyAction &r) {
    return l.action == r.action && l.data == r.data && l.modifiers == r.modifiers;
}

bool operator<(const HotkeyAction &l, const HotkeyAction &r) {
    if (l.action < r.action)
        return true;
    if (r.action < l.action)
        return false;
    if (l.data < r.data)
        return true;
    if (r.data < l.data)
        return false;
    return l.modifiers < r.modifiers;
}

QDataStream &operator<<(QDataStream &out, const HotkeyAction &v) {
    out << v.action;
    out << v.data;
    out << v.modifiers;
    return out;
}

QDataStream &operator>>(QDataStream &in, HotkeyAction &v) {
    in >> v.action;
    in >> v.data;
    in >> v.modifiers;
    return in;
}