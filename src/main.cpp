#include "widget.h"

#include <QApplication>
#include <QPlainTextEdit>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (Widget::debugView() == 0) {
        QByteArray localMsg = msg.toLocal8Bit();
        switch (type) {
        case QtDebugMsg:
            fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                    context.function);
            break;
        case QtInfoMsg:
            fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                    context.function);
            break;
        case QtWarningMsg:
            fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                    context.function);
            break;
        case QtCriticalMsg:
            fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                    context.function);
            break;
        case QtFatalMsg:
            fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line,
                    context.function);
            abort();
        }
    } else {
        switch (type) {
        case QtDebugMsg:
        case QtInfoMsg:
        case QtWarningMsg:
        case QtCriticalMsg:
            Widget::debugView()->appendPlainText(msg);
            break;
        case QtFatalMsg:
            abort();
        }
    }
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);

    QApplication a(argc, argv);
    Widget w;
    w.showMaximized();
    return a.exec();
}
