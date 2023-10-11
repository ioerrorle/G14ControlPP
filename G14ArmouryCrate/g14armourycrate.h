#ifndef G14ARMOURYCRATE_H
#define G14ARMOURYCRATE_H

#include <QMetaType>

enum class G14ArmouryCratePlan
{
    WindowsPerfomance = 0,
    TurboManual = 1,
    Silent = 2,
    Unknown = -1
};

Q_DECLARE_METATYPE(G14ArmouryCratePlan)

#endif // G14ARMOURYCRATE_H
