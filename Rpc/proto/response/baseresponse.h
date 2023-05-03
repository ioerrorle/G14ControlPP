#ifndef G14RPC_BASERESPONSE_H
#define G14RPC_BASERESPONSE_H

#include "../rpcmessagetype.h"
#include "error.h"

namespace g14rpc
{

struct BaseResponse
{
    MessageType type;
    RpcError error;
};

}

template<>
struct Props<g14rpc::BaseResponse>
{
    constexpr static auto properties = std::make_tuple(
        JSONPROP(g14rpc::BaseResponse, type),
        JSONPROP(g14rpc::BaseResponse, error)
    );
};

#endif // G14RPC_BASERESPONSE_H
