#pragma once

#include "historymodel.h"
#include "logger_utility.h"

#include <QString>

/**
 * Log a method, with all its parameters.
 */
#define LOG(name, ...) LoggerObject __loggerObject(name, ##__VA_ARGS__)

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
        if (m_model)
            m_model->logData(name);
        log(std::move(name));
    }

    template <typename... Ts>
    explicit LoggerObject(QString name, Ts... params)
        : LoggerObject()
    {
        if (m_model)
            m_model->logData(name, params...);

        QStringList paramList;
        (paramList.push_back(valueToString(params)), ...);
        QString result = name + " - " + paramList.join(", ");
        log(std::move(result));
    }

    ~LoggerObject();

private:
    friend class HistoryModel;

    LoggerObject();
    void log(QString &&string);

    inline static HistoryModel *m_model = nullptr;
};
