#pragma once

#include <QWidget>

#include <memory>

class TextDocument;
class ScriptRunner;
class QPlainTextEdit;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void run();
    void openFind();
    void closeFind();
    void find();

    static QPlainTextEdit *debugView() { return s_debugView; }

private:
    Ui::Widget *ui;
    std::unique_ptr<TextDocument> m_document;
    std::unique_ptr<ScriptRunner> m_scritpRunner;
    static inline QPlainTextEdit *s_debugView = nullptr;
    QString m_defaultFindText;
    bool m_defaultFindIsSelection = false;
    bool m_firstFind = false;
};
