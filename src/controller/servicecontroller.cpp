#include "servicecontroller.h"
#include "qdebug.h"

ServiceController::ServiceController() {
    QString error;
    bool result = mCpuController->init(error);
    if (!result) {
        qDebug() << "Init failed" << error;
        mCpuController = nullptr;
    }
}

void ServiceController::loadDefaultFanCurves(uchar *cpuTemps,
                                             uchar *cpuSpeeds,
                                             uchar *gpuTemps,
                                             uchar *gpuSpeeds) {}

void ServiceController::applyPowerPlanAndFansProfile(const uchar powerPlanId,
                                                     const FanCurve &cpuFanCurve,
                                                     const FanCurve &gpuFanCurve) {}

uchar ServiceController::getCurrentArCratePlanId() {
    return 0;
}

float ServiceController::getCpuTemp() const {
    return 0.0f;
}

void ServiceController::getFansRpms(long &cpuRpm, long &gpuRpm) {

}

/*float coreTempLimit;
float coreTempValue;

float apuTempLimit;
float apuTempValue;

float dgpuTempLimit;
float dgpuTempValue;

float setPoint;*/

CpuStatus ServiceController::getCpuStatus() const {
    if (mCpuController == nullptr) {
        return {0,0,0,0,0,0,0,0,0,0,0,0,0,0,95};
    }
    mCpuController->refreshTable();
    return {mCpuController->getStapmLimit(),
                mCpuController->getStapmValue(),
                mCpuController->getStapmTime(),
                mCpuController->getFastLimit(),
                mCpuController->getFastValue(),
                mCpuController->getSlowLimit(),
                mCpuController->getSlowValue(),
                mCpuController->getSlowTime(),
                mCpuController->getCoreTempLimit(),
                mCpuController->getCoreTemp(),
                mCpuController->getApuTempLimit(),
                mCpuController->getApuTemp(),
                mCpuController->getDGpuTempLimit(),
                mCpuController->getDGpuTemp(),
                mCpuController->getCclkSetpoint()};
}
