#include "error.h"

namespace Json {
    template<>
    g14rpc::ErrorCode asAny<g14rpc::ErrorCode>(const Value& value) {
        return g14rpc::ErrorCode(value.asInt());
    }
}
