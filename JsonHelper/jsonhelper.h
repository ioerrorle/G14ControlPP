#ifndef JSONHELPER_H
#define JSONHELPER_H

#include <QJsonObject>
#include <QJsonValue>
#include <utility>
/* usage
 * struct Hair {
 *     std::string color;
 *     int length;
 *
 *     bool operator==(const Hair& rhs) const {
 *         return std::tie(color, length) == std::tie(rhs.color, rhs.length);
 *     }
 * };
 *
 * template<>
 * struct Props<Hair>
 * {
 *     constexpr static auto properties = std::make_tuple(
 *         JSONPROP(Hair, color),
 *         JSONPROP(Hair, length)
 *     );
 * };
 *
 * struct Dog {
 *     std::string barkType;
 *     Hair hair;
 *     int weight = 0;
 *
 *     bool operator==(const Dog& rhs) const {
 *         return std::tie(barkType, hair, weight) == std::tie(rhs.barkType, rhs.hair, rhs.weight);
 *     }
 * };
 *
 * template<>
 * struct Props<Dog>
 * {
 *     constexpr static auto properties = std::make_tuple(
 *         JSONPROP(Dog, barkType),
 *         JSONPROP(Dog, hair),
 *         JSONPROP(Dog, weight)
 *     );
 * };
*/

#define JSONPROP(CLASS, MEMBER) property(&CLASS::MEMBER, #MEMBER)

namespace Json {
    template<typename T>
    T asAny(const QJsonValue&);
}

template<typename Class, typename T>
struct PropertyImpl {
    constexpr PropertyImpl(T Class::*aMember, const char* aName) : member{aMember}, name{aName} {}

    using Type = T;

    T Class::*member;
    const char* name;
};

template<typename Class, typename T>
constexpr PropertyImpl<Class, T> property(T Class::*member, const char* name) {
    return {member, name};
}

template <typename T>
struct Props
{
    constexpr static auto properties = std::tuple<>{};
};

template <typename T, T... S, typename F>
constexpr void for_sequence(std::integer_sequence<T, S...>, F&& f) {
    (static_cast<void>(f(std::integral_constant<T, S>{})), ...);
}

template<typename T>
T fromJson(const QJsonValue& data) {
    T object;

    // We first get the number of properties
    // constexpr auto nbProperties = std::tuple_size<decltype(T::properties)>::value;
    constexpr size_t propsCount = std::tuple_size<decltype(Props<T>::properties)>::value;

    if constexpr (propsCount == 0) { //means it's simple type
        return Json::asAny<T>(data);
    }

    // We iterate on the index sequence of size `nbProperties`
    for_sequence(std::make_index_sequence<propsCount>{}, [&](auto i) {
        // get the property
        constexpr auto property = std::get<i>(Props<T>::properties);

        // get the type of the property
        using Type = typename decltype(property)::Type;

        // set the value to the member
        // you can also replace `asAny` by `fromJson` to recursively serialize
        object.*(property.member) = fromJson<Type>(data[property.name]);
    });

    return object;
}

template<typename T>
QJsonValue toJson(const T& object) {
    constexpr auto propsCount = std::tuple_size<decltype(Props<T>::properties)>::value;

    if constexpr (std::is_enum_v<T>) {
        using RealType = std::underlying_type_t<T>;
        return static_cast<RealType>(object);
    } else if constexpr (propsCount == 0) {
        return object;
    } else {
        QJsonObject data;

        for_sequence(std::make_index_sequence<propsCount>{}, [&](auto i) {
            // get the property
            constexpr auto property = std::get<i>(Props<T>::properties);

            // set the value to the member
            data[property.name] = toJson(object.*(property.member));
        });

        return data;
    }
}

#endif //JSONHELPER_H
