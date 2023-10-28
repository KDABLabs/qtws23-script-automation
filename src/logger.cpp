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

LoggerObject::~LoggerObject() {}

LoggerObject::LoggerObject() {}

void LoggerObject::log(QString &&string) {
    qDebug() << string;
}
