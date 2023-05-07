#include "cpuprofile.h"

CpuProfile::CpuProfile(float stapmLimit, float stapmTime, float slowLimit, float slowTime, float fastLimit, CpuMode mode)
    : stapmLimit(stapmLimit)
    , stapmTime(stapmTime)
    , slowLimit(slowLimit)
    , slowTime(slowTime)
    , fastLimit(fastLimit)
    , mode(mode)
{

}

void CpuProfile::foo()
{

}

CpuState::CpuState()
    : CpuProfile()
{

}

A::A()
{

}
