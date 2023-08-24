#pragma once

#include <QWidget>

#include <memory>

class TextDocument;
class ScriptRunner;

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

private:
    Ui::Widget *ui;
    std::unique_ptr<TextDocument> m_document;
    std::unique_ptr<ScriptRunner> m_scritpRunner;
};
