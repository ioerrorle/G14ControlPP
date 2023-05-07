#include "cpustateresponse.h"

namespace Json {
    template<>
    CpuMode asAny<CpuMode>(const QJsonValue& value) {
        return CpuMode(value.toInt());
    }
}

namespace g14rpc
{

CpuStateResponse::CpuStateResponse()
    : BaseResponse{MessageType::GET_CPU_STATE, RpcError()}
{

}

}
