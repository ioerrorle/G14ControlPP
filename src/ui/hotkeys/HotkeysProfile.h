#ifndef G14CONTROLPP_HOTKEYSPROFILE_H
#define G14CONTROLPP_HOTKEYSPROFILE_H

#include <QString>
#include <QDataStream>
#include <QMap>
#include "HotkeyAction.h"

static const QMap<HotkeyActionCode, QString> HOTKEY_ACTION_LIST
{
    {HOTKEY_ACTION_NONE, "None"},
    {HOTKEY_ACTION_MUTE, "Mute"},
    {HOTKEY_ACTION_CALL_G14CPP, "Call G14cpp"},
    {HOTKEY_ACTION_CHANGE_POWER_PLAN, "Change power plan"},
    {HOTKEY_ACTION_LCD_BR_DOWN, "Lcd br down"},
    {HOTKEY_ACTION_LCD_BR_UP, "Lcd br up"},
    {HOTKEY_ACTION_TOGGLE_TOUCHPAD, "Toggle touchpad"},
    {HOTKEY_ACTION_SLEEP, "Sleep"},
    {HOTKEY_ACTION_PLANE_MODE, "Plane mode"},
    {HOTKEY_ACTION_KBD_BR_DOWN, "Kbd br down"},
    {HOTKEY_ACTION_KBD_BR_UP, "Kbd br up"},
    {HOTKEY_ACTION_BUTTON, "Button"}
};

typedef struct
{
    QString name;
    HotkeyAction mute;
    HotkeyAction rog;
    HotkeyAction fan;
    HotkeyAction lcdBrD;
    HotkeyAction lcdBrU;
    HotkeyAction touchpad;
    HotkeyAction sleep;
    HotkeyAction plane;
    HotkeyAction cButton;
    HotkeyAction vButton;
    HotkeyAction kbdBrD;
    HotkeyAction kbdBrU;
    HotkeyAction leftButton;
    HotkeyAction rightButton;
} HotkeysProfile;


bool operator==(const HotkeysProfile &l, const HotkeysProfile &r);

bool operator<(const HotkeysProfile &l, const HotkeysProfile &r);

QDataStream &operator<<(QDataStream &out, const HotkeysProfile &v);

QDataStream &operator>>(QDataStream &in, HotkeysProfile &v);

Q_DECLARE_METATYPE(HotkeysProfile);

static HotkeysProfile HOTKEY_PROFILE_DEFAULT = {
    "Default",
    {HOTKEY_ACTION_MUTE, 0x0, 0x0},
    {HOTKEY_ACTION_CALL_G14CPP, 0x0, 0x0},
    {HOTKEY_ACTION_CHANGE_POWER_PLAN, 0x0, 0x0},
    {HOTKEY_ACTION_LCD_BR_DOWN, 0x0, 0x0},
    {HOTKEY_ACTION_LCD_BR_UP, 0x0, 0x0},
    {HOTKEY_ACTION_TOGGLE_TOUCHPAD, 0x0, 0x0},
    {HOTKEY_ACTION_SLEEP, 0x0, 0x0},
    {HOTKEY_ACTION_PLANE_MODE, 0x0, 0x0},
    {HOTKEY_ACTION_NONE, 0x0, 0x0},
    {HOTKEY_ACTION_NONE, 0x0, 0x0},
    {HOTKEY_ACTION_KBD_BR_DOWN, 0x0, 0x0},
    {HOTKEY_ACTION_KBD_BR_UP, 0x0, 0x0},
    {HOTKEY_ACTION_NONE, 0x0, 0x0},
    {HOTKEY_ACTION_NONE, 0x0, 0x0}
};

static HotkeysProfile HOTKEY_PROFILE_CODER = {
    "Coder",
    {HOTKEY_ACTION_MUTE, 0x0, 0x0},
    {HOTKEY_ACTION_CALL_G14CPP, 0x0, 0x0},
    {HOTKEY_ACTION_CHANGE_POWER_PLAN, 0x0, 0x0},
    {HOTKEY_ACTION_LCD_BR_DOWN, 0x0, 0x0},
    {HOTKEY_ACTION_LCD_BR_UP, 0x0, 0x0},
    {HOTKEY_ACTION_TOGGLE_TOUCHPAD, 0x0, 0x0},
    {HOTKEY_ACTION_SLEEP, 0x0, 0x0},
    {HOTKEY_ACTION_PLANE_MODE, 0x0, 0x0},
    {HOTKEY_ACTION_KBD_BR_DOWN, 0x0, 0x0},
    {HOTKEY_ACTION_KBD_BR_UP, 0x0, 0x0},
    {HOTKEY_ACTION_BUTTON, 0x0151, 0x0},
    {HOTKEY_ACTION_BUTTON, 0x0149, 0x0},
    {HOTKEY_ACTION_BUTTON, 0x0147, 0x0},
    {HOTKEY_ACTION_BUTTON, 0x014f, 0x0}
};

#endif //G14CONTROLPP_HOTKEYSPROFILE_H
