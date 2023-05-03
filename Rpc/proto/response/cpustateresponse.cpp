#include "cpustateresponse.h"

namespace Json {
    template<>
    CpuMode asAny<CpuMode>(const Value& value) {
        return CpuMode(value.asInt());
    }
}

namespace g14rpc
{

CpuStateResponse::CpuStateResponse()
    : BaseResponse{MessageType::GET_CPU_STATE, RpcError()}
{

}

}
