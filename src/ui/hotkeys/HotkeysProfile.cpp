#include "HotkeysProfile.h"

bool operator==(const HotkeysProfile &l, const HotkeysProfile &r) {
    return l.name == r.name
    && l.mute == r.mute
    && l.rog == r.rog
    && l.fan == r.fan
    && l.lcdBrD == r.lcdBrD
    && l.lcdBrU == r.lcdBrU
    && l.touchpad == r.touchpad
    && l.sleep == r.sleep
    && l.plane == r.plane
    && l.cButton == r.cButton
    && l.vButton == r.vButton
    && l.kbdBrD == r.kbdBrD
    && l.kbdBrU == r.kbdBrU
    && l.leftButton == r.leftButton
    && l.rightButton == r.rightButton;
}

bool operator<(const HotkeysProfile &l, const HotkeysProfile &r) {
    if (l.name < r.name) return true; if (r.name < l.name) return false; 
    if (l.mute < r.mute) return true; if (r.mute < l.mute) return false; 
    if (l.rog < r.rog) return true; if (r.rog < l.rog) return false; 
    if (l.fan < r.fan) return true; if (r.fan < l.fan) return false; 
    if (l.lcdBrD < r.lcdBrD) return true; if (r.lcdBrD < l.lcdBrD) return false; 
    if (l.lcdBrU < r.lcdBrU) return true; if (r.lcdBrU < l.lcdBrU) return false; 
    if (l.touchpad < r.touchpad) return true; if (r.touchpad < l.touchpad) return false; 
    if (l.sleep < r.sleep) return true; if (r.sleep < l.sleep) return false; 
    if (l.plane < r.plane) return true; if (r.plane < l.plane) return false; 
    if (l.cButton < r.cButton) return true; if (r.cButton < l.cButton) return false; 
    if (l.vButton < r.vButton) return true; if (r.vButton < l.vButton) return false; 
    if (l.kbdBrD < r.kbdBrD) return true; if (r.kbdBrD < l.kbdBrD) return false; 
    if (l.kbdBrU < r.kbdBrU) return true; if (r.kbdBrU < l.kbdBrU) return false;
    if (l.leftButton < r.leftButton) return true; if (r.leftButton < l.leftButton) return false;
    return l.rightButton < r.rightButton;
}

QDataStream &operator<<(QDataStream &out, const HotkeysProfile &v) {
    out << v.name;
    out << v.mute;
    out << v.rog;
    out << v.fan;
    out << v.lcdBrD;
    out << v.lcdBrU;
    out << v.touchpad;
    out << v.sleep;
    out << v.plane;
    out << v.cButton;
    out << v.vButton;
    out << v.kbdBrD;
    out << v.kbdBrU;
    out << v.leftButton;
    out << v.rightButton;
    return out;
}

QDataStream &operator>>(QDataStream &in, HotkeysProfile &v) {
    in >> v.name;
    in >> v.mute;
    in >> v.rog;
    in >> v.fan;
    in >> v.lcdBrD;
    in >> v.lcdBrU;
    in >> v.touchpad;
    in >> v.sleep;
    in >> v.plane;
    in >> v.cButton;
    in >> v.vButton;
    in >> v.kbdBrD;
    in >> v.kbdBrU;
    in >> v.leftButton;
    in >> v.rightButton;
    return in;
}
