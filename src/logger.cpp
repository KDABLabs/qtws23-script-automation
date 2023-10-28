#include "logger.h"

#include <QDebug>

LoggerDisabler::LoggerDisabler()
    : m_originalCanLog(LoggerObject::m_canLog)
{
    LoggerObject::m_canLog = false;
}

LoggerDisabler::~LoggerDisabler()
{
    LoggerObject::m_canLog = m_originalCanLog;
}

LoggerObject::LoggerObject()
    : m_firstLogger(m_canLog)
{
}

LoggerObject::~LoggerObject()
{
    if (m_firstLogger)
        m_canLog = true;
}

void LoggerObject::log(QString &&string) {
    qDebug() << string;
    m_canLog = false;
}
