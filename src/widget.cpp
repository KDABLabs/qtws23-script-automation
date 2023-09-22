#include "widget.h"
#include "ui_widget.h"
#include "textdocument.h"
#include "scriptrunner.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    s_debugView = ui->debugView;
    m_document.reset(new TextDocument(ui->editor));
    m_scritpRunner.reset(new ScriptRunner(m_document.get()));

    connect(ui->runButton, &QToolButton::clicked, this, &Widget::run);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::run()
{
    const auto &script = ui->script->toPlainText();
    m_scritpRunner->runScript(script);
}