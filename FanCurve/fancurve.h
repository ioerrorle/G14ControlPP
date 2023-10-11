#ifndef FANCURVE_H
#define FANCURVE_H

#include <QMap>
#include <QMetaType>

struct FanCurve
{
    QString name;
    QMap<int, int> tempToSpeedMap;
    bool isEmpty = true;
};

#endif // FANCURVE_H
