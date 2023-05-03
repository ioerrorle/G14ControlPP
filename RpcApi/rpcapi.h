#ifndef RPCAPI_H
#define RPCAPI_H

#include <QString>
#include "CpuProfile/cpuprofile.h"

namespace RpcApi
{
    enum class ErrorCode
    {

    };

    struct Error
    {
        int code;
        QString message;
    };

    struct BaseResponse
    {
        Error error;
        virtual QString serialize() const;
        static BaseResponse fromJson(const QString &json);
    };

    struct CpuStateResponse : public BaseResponse
    {
        CpuState cpuState;
    };
}

#endif // RPCAPI_H
