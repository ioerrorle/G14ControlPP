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

    bool operator==(const CpuProfile &other);
    bool operator!=(const CpuProfile &other);
};

//Q_DECLARE_METATYPE(CpuProfile)

struct CpuState : public CpuProfile
{
    CpuState();
    CpuState(const CpuProfile &profile);

    float coreTempLimit = 0;
    float coreTemp = 0;
    float apuTempLimit = 0;
    float apuTemp = 0;
    float dGpuTempLimit = 0;
    float dGpuTemp = 0;

    bool operator==(const CpuState &other);
    bool operator!=(const CpuState &other);

};

//Q_DECLARE_METATYPE(CpuState)

#endif // CPUPROFILE_H
