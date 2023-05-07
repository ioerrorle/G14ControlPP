#ifndef G14RPC_CPUSTATERESPONSE_H
#define G14RPC_CPUSTATERESPONSE_H

#include "CpuProfile/cpuprofile.h"

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

namespace g14rpc
{

struct CpuStateResponse : public BaseResponse
{
    CpuStateResponse();

    CpuState cpuState;
};

}

template<>
struct Props<g14rpc::CpuStateResponse> {
    constexpr static auto properties = std::tuple_cat(Props<g14rpc::BaseResponse>::properties, std::make_tuple(
        JSONPROP(g14rpc::CpuStateResponse, cpuState)
    ));
};

#endif // G14RPC_CPUSTATERESPONSE_H
