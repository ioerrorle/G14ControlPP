#include "FansProfile.h"

FansProfile::FansProfile(const QString &name, const FanCurve &cpu, const FanCurve &gpu)
        : name(name), cpu(cpu), gpu(gpu) {

}

FansProfile::FansProfile() {
    empty = true;
}

const QString &FansProfile::getName() const {
    return name;
}

const FanCurve &FansProfile::getCpu() const {
    return cpu;
}

const FanCurve &FansProfile::getGpu() const {
    return gpu;
}

bool FansProfile::isEmpty() const {
    return empty;
}

void FansProfile::getCpuAcpiData(uchar *result) {
    uchar prefix[] = {FAN_CPU, 0x00, 0x11, 0x00};
    memcpy(result, prefix, 4);
    cpu.toAcpiData(&result[4]);
}

void FansProfile::getGpuAcpiData(uchar *result) {
    uchar prefix[] = {FAN_GPU, 0x00, 0x11, 0x00};
    memcpy(result, prefix, 4);
    gpu.toAcpiData(&result[4]);
}

FansProfile FansProfile::fromQStringList(const QString &name, const QStringList &list) {
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

    return FansProfile(name, FanCurve(cpuTemps, cpuSpeeds), FanCurve(gpuTemps, gpuSpeeds));
}

QStringList FansProfile::toQStringList() {
    QStringList result;
    for (int i = 0; i < 8; i++) {
        result.append(QString::asprintf("%d:%d", cpu.getTemp()[i], cpu.getSpeed()[i]));
    }
    for (int i = 0; i < 8; i++) {
        result.append(QString::asprintf("%d:%d", gpu.getTemp()[i], gpu.getSpeed()[i]));
    }
}
