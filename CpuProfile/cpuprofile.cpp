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

bool CpuProfile::operator==(const CpuProfile &other)
{
    return stapmLimit == other.stapmLimit &&
            stapmTime == other.stapmTime &&
            slowLimit == other.slowLimit &&
            slowTime == other.slowTime &&
            fastLimit == other.fastLimit;
}

bool CpuProfile::operator!=(const CpuProfile &other)
{
    return !(*this == other);
}

CpuState::CpuState()
    : CpuProfile()
{

}

CpuState::CpuState(const CpuProfile &profile)
    : CpuProfile(profile)
{

}

bool CpuState::operator==(const CpuState &other)
{
    return static_cast<CpuState>(*this) == static_cast<CpuState>(other) &&
            coreTempLimit == other.coreTempLimit &&
            coreTemp == other.coreTemp &&
            apuTempLimit == other.apuTempLimit &&
            apuTemp == other.apuTemp &&
            dGpuTempLimit == other.dGpuTempLimit &&
            dGpuTemp == other.dGpuTemp;
}

bool CpuState::operator!=(const CpuState &other)
{
    return !(*this == other);
}
