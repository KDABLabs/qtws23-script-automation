#include "widget.h"
#include "ui_widget.h"
#include "textdocument.h"
#include "scriptrunner.h"

#include <QShortcut>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    s_debugView = ui->debugView;
    m_document.reset(new TextDocument(ui->editor));
    m_scritpRunner.reset(new ScriptRunner(m_document.get()));

    connect(ui->runButton, &QToolButton::clicked, this, &Widget::run);
    connect(ui->findNextButton, &QToolButton::clicked, this, &Widget::find);

    auto openFindShortcut = new QShortcut(QKeySequence("Ctrl+F"), this);
    connect(openFindShortcut, &QShortcut::activated, this, &Widget::openFind);
    auto closeFindShortcut = new QShortcut(QKeySequence(Qt::Key_Escape), this);
    connect(closeFindShortcut, &QShortcut::activated, this, &Widget::closeFind);

    ui->findWidget->hide();
    ui->editor->setFocus();
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

void Widget::openFind()
{
    ui->findWidget->setVisible(true);

    m_defaultFindText = m_document->selectedText();
    m_defaultFindIsSelection = true;
    if (m_defaultFindText.isEmpty()) {
        m_defaultFindText = m_document->currentWord();
        m_defaultFindIsSelection = false;
    }
    ui->findEdit->setText(m_defaultFindText);
    ui->findEdit->selectAll();
    ui->findEdit->setFocus();
    m_firstFind = true;
}

void Widget::closeFind()
{
    ui->findWidget->setVisible(false);
    ui->editor->setFocus();
}

void Widget::find()
{
    const QString &text = ui->findEdit->text();

    // Enable logging and replay of selected text or word, only the first time
    // Following find or replace won't call this
    if (m_firstFind) {
        if (text == m_defaultFindText) {
            if (m_defaultFindIsSelection)
                m_document->selectedText();
            else
                m_document->currentWord();
        }
        m_firstFind = false;
    }

    m_document->find(text);
}