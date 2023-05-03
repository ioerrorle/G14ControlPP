#ifndef G14RPC_ERROR_H
#define G14RPC_ERROR_H

#include <QMetaType>
#include <JsonHelper/jsonhelper.h>

namespace g14rpc
{

enum class ErrorCode
{
    OK = 0,
    SEND_FAILED = 1,
};

struct RpcError
{
    ErrorCode code = ErrorCode::OK;
    QString message;
};

}

template<>
struct Props<g14rpc::RpcError>
{
    constexpr static auto properties = std::make_tuple(
        JSONPROP(g14rpc::RpcError, code),
        JSONPROP(g14rpc::RpcError, message)
    );
};

Q_DECLARE_METATYPE(g14rpc::ErrorCode)
Q_DECLARE_METATYPE(g14rpc::RpcError)

#endif // G14RPC_ERROR_H
