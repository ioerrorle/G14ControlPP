#ifndef G14RPC_BASEREQUEST_H
#define G14RPC_BASEREQUEST_H

#include <JsonHelper/jsonhelper.h>

#include "../rpcmessagetype.h"

namespace g14rpc {

struct BaseRequest {

    MessageType type = MessageType::UNKNOWN;
};

}

template<>
struct Props<g14rpc::BaseRequest>
{
    constexpr static auto properties = std::make_tuple(
        JSONPROP(g14rpc::BaseRequest, type)
    );
};

#endif // G14RPC_BASEREQUEST_H
