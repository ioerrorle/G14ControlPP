#include "jsonhelper.h"

namespace Json
{
    template<>
    int asAny<int>(const QJsonValue& value) {
        return value.toInt();
    }

    template<>
    float asAny<float>(const QJsonValue& value) {
        return value.toDouble();
    }

    template<>
    QString asAny<QString>(const QJsonValue& value) {
        return value.toString();
    }
}
