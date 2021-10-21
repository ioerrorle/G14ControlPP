#ifndef G14CONTROLPP_HOTKEYACTION_H
#define G14CONTROLPP_HOTKEYACTION_H

#include <QDataStream>

typedef struct {
    uchar action;
    quint32 data;
    quint32 modifiers;
} HotkeyAction;

bool operator==(const HotkeyAction &l, const HotkeyAction &r);

bool operator<(const HotkeyAction &l, const HotkeyAction &r);

QDataStream &operator<<(QDataStream &out, const HotkeyAction &v);

QDataStream &operator>>(QDataStream &in, HotkeyAction &v);

Q_DECLARE_METATYPE(HotkeyAction);


#endif //G14CONTROLPP_HOTKEYACTION_H
