#ifndef G14CONTROLPP_FANSPROFILE_H
#define G14CONTROLPP_FANSPROFILE_H

#include "src/model/FanCurve.h"
#include <QStringList>

#define FAN_CPU 0x24
#define FAN_GPU 0x25

class FansProfile {
public:
    FansProfile(const QString &name, const FanCurve &cpu, const FanCurve &gpu);
    FansProfile();

    const QString &getName() const;

    const FanCurve &getCpu() const;

    const FanCurve &getGpu() const;

    bool getIsDefault() const;

    void getCpuAcpiData(uchar *result);

    void getGpuAcpiData(uchar *result);

    FansProfile withName(const QString &name);



    static const FansProfile Default;
    static const QString CurrentFansProfileName;
    static const QString DefaultFansProfileName;

    static FansProfile fromQStringList(const QString &name, const QStringList &list);

    const QStringList toQStringList() const;

private:
    QString name;
    FanCurve cpu;
    FanCurve gpu;
    bool isDefault = true;
};

Q_DECLARE_METATYPE(FansProfile)

#endif //G14CONTROLPP_FANSPROFILE_H
