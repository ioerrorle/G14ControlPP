#ifndef G14CONTROLPP_FANSPROFILE_H
#define G14CONTROLPP_FANSPROFILE_H

#include <QCoreApplication>
#include <QStringList>
#include "src/model/FanCurve.h"

#define FAN_CPU 0x24
#define FAN_GPU 0x25

namespace Fans {


class Profile {

    Q_DECLARE_TR_FUNCTIONS(Fans::Profile);
public:
    QString name;
    Curve cpu;
    Curve gpu;
    bool isDefault = true;

    Profile(const QString &name, const QStringList &list) {
        this->name = name;

        if (list.size() < 16) {
            return;
        }

        uchar cpuTemps[8];
        uchar cpuSpeeds[8];
        uchar gpuTemps[8];
        uchar gpuSpeeds[8];

        for (int i = 0; i < 8; i++) {
            QStringList cpuPair = list[i].split(":");
            QStringList gpuPair = list[i + 8].split(":");
            cpuTemps[i] = cpuPair[0].toInt();
            cpuSpeeds[i] = cpuPair[1].toInt();
            gpuTemps[i] = gpuPair[0].toInt();
            gpuSpeeds[i] = gpuPair[1].toInt();
        }
    }

    Profile(const QString &name, const Curve &cpu, const Curve &gpu)
        : name(name)
        , cpu(cpu)
        , gpu(gpu)
        , isDefault(false) { }

    Profile() { }

    Profile& withName(const QString &name) {
        this->name = name;
        return *this;
    };

    static const Profile Default;
    static const QString CurrentFansProfileName;
    static const QString DefaultFansProfileName;

    const QStringList toQStringList() const {
        if (isDefault) {
            QStringList defResult;
            defResult.append("default");
            return defResult;
        }

        QStringList result(16);
        for (int i = 0; i < 8; i++) {
            result[i] = QString::asprintf("%d:%d", cpu.temp[i], cpu.temp[i]);
            result[i+8] = QString::asprintf("%d:%d", gpu.temp[i], gpu.speed[i]);
        }

        return result;
    };
};

}

Q_DECLARE_METATYPE(Fans::Profile)

#endif //G14CONTROLPP_FANSPROFILE_H
