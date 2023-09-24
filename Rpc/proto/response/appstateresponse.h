#ifndef G14RPC_APPSTATERESPONSE_H
#define G14RPC_APPSTATERESPONSE_H

#include "G14AppState/appstate.h"
#include "baseresponse.h"

template<>
struct Props<CpuProfile>
{
    constexpr static auto properties = std::make_tuple(
        JSONPROP(CpuProfile, stapmLimit),
        JSONPROP(CpuProfile, stapmTime),
        JSONPROP(CpuProfile, slowLimit),
        JSONPROP(CpuProfile, slowTime),
        JSONPROP(CpuProfile, fastLimit),
        JSONPROP(CpuProfile, mode)
    );
};

template<>
struct Props<CpuState> {
    constexpr static auto properties = std::tuple_cat(Props<CpuProfile>::properties, std::make_tuple(
        JSONPROP(CpuState, coreTempLimit),
        JSONPROP(CpuState, coreTemp),
        JSONPROP(CpuState, apuTempLimit),
        JSONPROP(CpuState, apuTemp),
        JSONPROP(CpuState, dGpuTempLimit),
        JSONPROP(CpuState, dGpuTemp)
    ));
};

template<>
struct Props<AppState> {
    constexpr static auto properties = std::make_tuple(
            JSONPROP(AppState, arCratePlan),
            JSONPROP(AppState, cpuState)
            );
};

namespace g14rpc
{

struct AppStateResponse : public BaseResponse
{
    AppStateResponse();

    AppState appState;
};

}

Q_DECLARE_METATYPE(g14rpc::AppStateResponse)


template<>
struct Props<g14rpc::AppStateResponse> {
    constexpr static auto properties = std::tuple_cat(
        Props<g14rpc::BaseResponse>::properties, std::make_tuple(
            JSONPROP(g14rpc::AppStateResponse, appState)
    ));
};

#endif // G14RPC_APPSTATERESPONSE_H
