#include "rpcmessagetype.h"
#include <JsonHelper/jsonhelper.h>

namespace Json {
    template<>
    g14rpc::MessageType asAny<g14rpc::MessageType>(const QJsonValue& value) {
        return g14rpc::MessageType(value.toInt());
    }
}
