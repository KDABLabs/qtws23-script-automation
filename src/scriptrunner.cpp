#include "scriptrunner.h"
#include "textdocument.h"

#include <QQmlComponent>
#include <QtQml/private/qqmlengine_p.h>

ScriptRunner::ScriptRunner(TextDocument *document, QObject *parent)
    : QObject(parent)
{
    m_engine = new QQmlEngine(this);
}

ScriptRunner::~ScriptRunner() { }

void ScriptRunner::runScript(const QString &script)
{
    qDebug() << "==> Start script";

    m_hasError = false;
    runJavascript(script);

    qDebug() << "<== End script";
}

void ScriptRunner::runJavascript(const QString &script)
{
    const QString text = QStringLiteral("import QtQml 2.12\n"
                                        "import com.kdab.script 1.0\n"
                                        "QtObject { function run() { %1 } }")
                             .arg(script);

    QQmlComponent component(m_engine);
    component.setData(text.toLatin1(), {});

    QObject *scriptObject = component.create();
    m_errors = component.errors();
    m_hasError = component.isError();
    if (component.isReady() && !component.isError())
        QMetaObject::invokeMethod(scriptObject, "run");
}
