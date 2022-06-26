#ifndef G14CONTROLPP_HOTKEYACTION_H
#define G14CONTROLPP_HOTKEYACTION_H

#include <QDataStream>
#include <QMetaType>

enum HotkeyActionCode {
    HOTKEY_ACTION_RELEASE = 0xFF,
    HOTKEY_ACTION_NONE = 0x00,
    HOTKEY_ACTION_MUTE = 0x01,
    HOTKEY_ACTION_CALL_G14CPP = 0x02,
    HOTKEY_ACTION_CHANGE_POWER_PLAN = 0x03,
    HOTKEY_ACTION_LCD_BR_DOWN =  0x04,
    HOTKEY_ACTION_LCD_BR_UP = 0x05,
    HOTKEY_ACTION_TOGGLE_TOUCHPAD = 0x06,
    HOTKEY_ACTION_SLEEP = 0x07,
    HOTKEY_ACTION_PLANE_MODE = 0x08,
    HOTKEY_ACTION_KBD_BR_DOWN = 0x09,
    HOTKEY_ACTION_KBD_BR_UP = 0x0A,
    HOTKEY_ACTION_BUTTON = 0xFE
};

struct HotkeyAction {
    HotkeyActionCode action;
    quint32 data;
    quint32 modifiers;
} ;

bool operator==(const HotkeyAction &l, const HotkeyAction &r);

bool operator<(const HotkeyAction &l, const HotkeyAction &r);

QDataStream &operator<<(QDataStream &out, const HotkeyAction &v);

QDataStream &operator>>(QDataStream &in, HotkeyAction &v);

Q_DECLARE_METATYPE(HotkeyAction);


#endif //G14CONTROLPP_HOTKEYACTION_H
