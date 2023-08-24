#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QSharedPointer>
#include <QString>

class TextDocument;

class ScriptRunner : public QObject
{
    Q_OBJECT

public:
    explicit ScriptRunner(TextDocument *document, QObject *parent = nullptr);
    ~ScriptRunner();

    void runScript(const QString &script);

    bool hasError() const { return m_hasError; }
    QList<QQmlError> errors() const { return m_errors; }

private:
    void runJavascript(const QString &script);

private:
    bool m_hasError = false;
    QList<QQmlError> m_errors;
    QQmlEngine *m_engine = nullptr;
};
