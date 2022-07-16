#ifndef SERVICECONTROLLER_H
#define SERVICECONTROLLER_H

#include <qglobal.h>

#include "src/controller/cpu/cpucontroller.h"
#include "src/model/FanCurve.h"
#include "src/model/CpuProfile.h"

namespace Ryzen
{

struct Status : Profile {

    float stapmValue = 0;
    float fastValue = 0;
    float slowValue = 0;

    float coreTempLimit = 0;
    float coreTempValue = 0;

    float apuTempLimit = 0;
    float apuTempValue = 0;

    float dgpuTempLimit = 0;
    float dgpuTempValue = 0;

    Status(float stapmLimit,
           float stapmTime,
           float slowLimit,
           float slowTime,
           float fastLimit,
           int mode,
           float stapmValue,
           float fastValue,
           float slowValue,
           float coreTempLimit,
           float coreTempValue,
           float apuTempLimit,
           float apuTempValue,
           float dgpuTempLimit,
           float dgpuTempValue)
        : Profile(QString(), stapmLimit, stapmTime, slowLimit, slowTime, fastLimit, mode)
        , stapmValue(stapmValue)
        , fastValue(fastValue)
        , slowValue(slowValue)
        , coreTempLimit(coreTempLimit)
        , coreTempValue(coreTempValue)
        , apuTempLimit(apuTempLimit)
        , apuTempValue(apuTempValue)
        , dgpuTempLimit(dgpuTempLimit)
        , dgpuTempValue(dgpuTempValue)
    { };

    //const CpuProfile createCpuProfile() {
    //    return CpuProfile("", stapmLimit, stapmTime, slowLimit, slowTime, fastLimit, setPoint == 95.0f ? SP_POWER_SAVING : SP_PERFORMANCE);
    //}
};

}

class ServiceController {
public:
    ServiceController();

    void loadDefaultFanCurves(uchar *cpuTemps,
                              uchar *cpuSpeeds,
                              uchar *gpuTemps,
                              uchar *gpuSpeeds);
    //void applyPowerPlanAndFansProfile(const uchar powerPlanId, const FanCurve &cpuFanCurve, const FanCurve &gpuFanCurve);

    uchar getCurrentArCratePlanId();

    float getCpuTemp() const;

    void getFansRpms(long &cpuRpm, long &gpuRpm);

    Ryzen::Status getCpuStatus() const;

    void reapplyArCratePlan();

    void applyCpuProfile(const Ryzen::Profile &cpuProfile);

private:

    CpuController *mCpuController = nullptr;//new CpuController();
};

#endif // SERVICECONTROLLER_H
