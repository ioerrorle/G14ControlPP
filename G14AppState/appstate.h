#ifndef G14APPSTATE_H
#define G14APPSTATE_H

#include "G14ArmouryCrate/g14armourycrate.h"
#include "CpuProfile/cpuprofile.h"

struct AppState
{
    G14ArmouryCratePlan arCratePlan;
    CpuState cpuState;
};

#endif //G14APPSTATE_H
