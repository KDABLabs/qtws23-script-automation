#include "logger.h"

#include <QDebug>

LoggerObject::~LoggerObject() {}

LoggerObject::LoggerObject() {}

void LoggerObject::log(QString &&string) {
    qDebug() << string;
}
