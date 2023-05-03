#include "jsonhelper.h"

namespace Json {
    template<>
    int asAny<int>(const Value& value) {
        return value.asInt();
    }

    template<>
    float asAny<float>(const Value& value) {
        return value.asFloat();
    }

    template<>
    std::string asAny<std::string>(const Value& value) {
        return value.asString();
    }
}
