#pragma once

#include "historymodel.h"
#include "logger_utility.h"

#include <QString>

/**
 * Log a method, with all its parameters.
 */
#define LOG(name, ...) LoggerObject __loggerObject(name, ##__VA_ARGS__)

/**
 * Create a parameter, the name will depend on the type
 */
#define LOG_ARG(name, value) LoggerArg(name, value)

/**
 * Save the returned value, the name will depend on the type
 */
#define LOG_RETURN(name, value)                                                                                        \
    do {                                                                                                               \
        const auto &__value = value;                                                                                   \
        __loggerObject.setReturnValue(name, __value);                                                                  \
        return __value;                                                                                                \
    } while (false)

/**
 * @brief The LoggerDisabler class is a RAII class to temporary disable logging
 */
class LoggerDisabler
{
public:
    LoggerDisabler();
    ~LoggerDisabler();

private:
    bool m_originalCanLog = true;
};

/**
 * @brief The LoggerObject class is a utility class to help logging API calls
 *
 * This class ensure that only the first API call is logged, subsequent calls done by the first one won't.
 * Do not use this class directly, but use the macros LOG and LOG_AND_MERGE
 */
class LoggerObject
{
public:
    explicit LoggerObject(QString name)
        : LoggerObject()
    {
        if (!m_canLog)
            return;
        if (m_model)
            m_model->logData(name);
        log(std::move(name));
    }

    template <typename... Ts>
    explicit LoggerObject(QString name, Ts... params)
        : LoggerObject()
    {
        if (!m_canLog)
            return;
        if (m_model)
            m_model->logData(name, params...);

        QStringList paramList;
        (paramList.push_back(valueToString(params)), ...);
        QString result = name + " - " + paramList.join(", ");
        log(std::move(result));
    }

    ~LoggerObject();

    template <typename T>
    void setReturnValue(QString &&name, const T &value)
    {
        if (m_firstLogger && m_model)
            m_model->setReturnValue(std::move(name), value);
    }

private:
    friend class HistoryModel;
    friend class LoggerDisabler;

    LoggerObject();
    void log(QString &&string);

    inline static bool m_canLog = true;
    bool m_firstLogger = false;

    inline static HistoryModel *m_model = nullptr;
};
