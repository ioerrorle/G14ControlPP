#ifndef G14RPC_APPSTATEREQUEST_H
#define G14RPC_APPSTATEREQUEST_H

#include "baserequest.h"

namespace g14rpc {

struct AppStateRequest : public BaseRequest
{
    AppStateRequest();
};

}

template<>
struct Props<g14rpc::AppStateRequest>
{
    constexpr static auto properties = Props<g14rpc::BaseRequest>::properties;
};

#endif // G14RPC_APPSTATEREQUEST_H
