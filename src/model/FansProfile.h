#ifndef G14CONTROLPP_FANSPROFILE_H
#define G14CONTROLPP_FANSPROFILE_H

#include <QStringList>
#include "src/model/FanCurve.h"

#define FAN_CPU 0x24
#define FAN_GPU 0x25

class FansProfile {

public:
    FansProfile(const QString &name, const FanCurve &cpu, const FanCurve &gpu);
    FansProfile();

    const QString &getName() const;

    const FanCurve &getCpu() const;

    const FanCurve &getGpu() const;

    bool isEmpty() const;

    void getCpuAcpiData(uchar *result);

    void getGpuAcpiData(uchar *result);

    static FansProfile fromQStringList(const QString &name, const QStringList &list);

    const QStringList toQStringList() const;

    void setName(const QString &name);

private:
    QString name;
    FanCurve cpu;
    FanCurve gpu;
    bool empty = false;
};

#endif //G14CONTROLPP_FANSPROFILE_H
