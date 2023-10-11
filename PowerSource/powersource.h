#ifndef POWERSOURCE_H
#define POWERSOURCE_H

#include <QMetaType>

enum class PowerSource
{
    Battery = 0,
    TypeC = 1,
    Charger = 2
};

Q_DECLARE_METATYPE(PowerSource)

#endif // POWERSOURCE_H
