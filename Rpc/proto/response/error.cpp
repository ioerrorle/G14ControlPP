#include "error.h"

namespace Json {
    template<>
    g14rpc::ErrorCode asAny<g14rpc::ErrorCode>(const QJsonValue& value) {
        return g14rpc::ErrorCode(value.toInt());
    }
}
