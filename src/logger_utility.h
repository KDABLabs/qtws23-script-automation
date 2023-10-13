#pragma once

#include <QMetaEnum>
#include <QString>

#include <concepts>

template <typename T>
concept HasToString = requires(const T &t)
{
    t.toString();
};

template <typename T>
concept HasPointerToString = requires(const T &t)
{
    t->toString();
};

/**
 * @brief toString
 * Returns a string for any kind of data you can pass as a parameter.
 */
template <class T>
QString valueToString(const T &data)
{
    if constexpr (std::is_same_v<std::remove_cvref_t<T>, QString>) {
        QString text = data;
        text.replace('\n', "\\n");
        text.replace('\t', "\\t");
        return text;
    } else if constexpr (std::is_same_v<std::remove_cvref_t<T>, bool>)
        return data ? "true" : "false";
    else if constexpr (std::is_floating_point_v<T> || std::is_integral_v<T>)
        return QString::number(data);
    else if constexpr (std::is_same_v<std::remove_cvref_t<T>, QStringList>)
        return '{' + data.join(", ") + '}';
    else if constexpr (HasToString<T>)
        return data.toString();
    else if constexpr (HasPointerToString<T>)
        return data->toString();
    else
        Q_UNREACHABLE();
    return {};
}

