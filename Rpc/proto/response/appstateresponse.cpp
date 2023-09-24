#include "appstateresponse.h"

namespace Json {
    template<>
    CpuMode asAny<CpuMode>(const QJsonValue& value) {
        return CpuMode(value.toInt());
    }

    template<>
    G14ArmouryCratePlan asAny<G14ArmouryCratePlan>(const QJsonValue& value) {
        return G14ArmouryCratePlan(value.toInt());
    }
}

namespace g14rpc
{

AppStateResponse::AppStateResponse()
    : BaseResponse{MessageType::APP_STATE, RpcError()}
{

}

}
