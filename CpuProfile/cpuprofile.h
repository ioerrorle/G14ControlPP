#ifndef CPUPROFILE_H
#define CPUPROFILE_H

#include <QMetaType>

enum class CpuMode
{
    Performance = 0,
    PowerSaving = 1,
    Default = 2,

    //count_,
    //last = count_ - 1,
    //first_ = 0
};

struct A
{
    A();
};

struct CpuProfile
{
    CpuProfile() = default;
    CpuProfile(float stapmLimit,
               float stapmTime,
               float slowLimit,
               float slowTime,
               float fastLimit,
               CpuMode mode);
    void foo();

    float stapmLimit;
    float stapmTime;
    float slowLimit;
    float slowTime;
    float fastLimit;
    CpuMode mode;
};

//Q_DECLARE_METATYPE(CpuProfile)

struct CpuState : public CpuProfile
{
    CpuState();

    float coreTempLimit;
    float coreTemp;
    float apuTempLimit;
    float apuTemp;
    float dGpuTempLimit;
    float dGpuTemp;
};

//Q_DECLARE_METATYPE(CpuState)

#endif // CPUPROFILE_H
