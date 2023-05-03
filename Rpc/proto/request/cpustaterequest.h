#ifndef G14RPC_CPUSTATEREQUEST_H
#define G14RPC_CPUSTATEREQUEST_H

#include "baserequest.h"

namespace g14rpc {

struct CpuStateRequest : public BaseRequest
{
    CpuStateRequest();
};

}

template<>
struct Props<g14rpc::CpuStateRequest>
{
    constexpr static auto properties = Props<g14rpc::BaseRequest>::properties;
};

#endif // G14RPC_CPUSTATEREQUEST_H
