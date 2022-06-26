#ifndef SERVICECONTROLLER_H
#define SERVICECONTROLLER_H

#include <qglobal.h>

#include "src/controller/cpu/cpucontroller.h"
#include "src/model/FanCurve.h"
#include "src/model/CpuProfile.h"

struct CpuStatus {
    float stapmLimit;
    float stapmValue;
    float stapmTime;

    float fastLimit;
    float fastValue;

    float slowLimit;
    float slowValue;
    float slowTime;

    float coreTempLimit;
    float coreTempValue;

    float apuTempLimit;
    float apuTempValue;

    float dgpuTempLimit;
    float dgpuTempValue;

    float setPoint;

    const CpuProfile createCpuProfile() {
        return CpuProfile("", stapmLimit, stapmTime, slowLimit, slowTime, fastLimit, setPoint == 95.0f ? SP_POWER_SAVING : SP_PERFORMANCE);
    }
};

class ServiceController {
public:
    ServiceController();

    void loadDefaultFanCurves(uchar *cpuTemps,
                              uchar *cpuSpeeds,
                              uchar *gpuTemps,
                              uchar *gpuSpeeds);
    void applyPowerPlanAndFansProfile(const uchar powerPlanId, const FanCurve &cpuFanCurve, const FanCurve &gpuFanCurve);

    uchar getCurrentArCratePlanId();

    float getCpuTemp() const;

    void getFansRpms(long &cpuRpm, long &gpuRpm);

    CpuStatus getCpuStatus() const;

    void reapplyArCratePlan();

    void applyCpuProfile(const CpuProfile &cpuProfile);

private:

    CpuController *mCpuController = new CpuController();
};

#endif // SERVICECONTROLLER_H
