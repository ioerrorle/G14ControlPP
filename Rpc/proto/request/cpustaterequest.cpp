#include "cpustaterequest.h"

namespace g14rpc
{

CpuStateRequest::CpuStateRequest()
    : BaseRequest{MessageType::GET_CPU_STATE}
{

}

}
