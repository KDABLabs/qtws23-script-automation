#include "textdocument.h"

#include "logger.h"

#include <QPlainTextEdit>
#include <private/qwidgettextcontrol_p.h>

TextDocument::TextDocument(QPlainTextEdit *textEdit, QObject *parent)
    : QObject(parent)
    , m_document(textEdit)
{
    Q_ASSERT(textEdit);
    connect(m_document, &QPlainTextEdit::selectionChanged, this, &TextDocument::selectionChanged);
    connect(m_document, &QPlainTextEdit::cursorPositionChanged, this, &TextDocument::positionChanged);
    m_document->installEventFilter(this);
    m_instance = this;
}

TextDocument::~TextDocument() = default;

TextDocument *TextDocument::create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
{
    QJSEngine::setObjectOwnership(m_instance, QJSEngine::CppOwnership);
    return m_instance;
}

QString TextDocument::currentWord() const
{
    LOG("TextDocument::currentWord");
    QTextCursor cursor = m_document->textCursor();
    cursor.movePosition(QTextCursor::StartOfWord);
    cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
    return cursor.selectedText();
}

QString TextDocument::selectedText() const
{
    LOG("TextDocument::selectedText");
    // Replace \u2029 with \n
    const QString text = m_document->textCursor().selectedText().replace(QChar(8233), "\n");
    return text;
}

bool TextDocument::hasSelection() const
{
    return m_document->textCursor().hasSelection();
}

void TextDocument::movePosition(QTextCursor::MoveOperation operation, QTextCursor::MoveMode mode, int count)
{
    auto cursor = m_document->textCursor();
    cursor.movePosition(operation, mode, count);
    m_document->setTextCursor(cursor);
}

void TextDocument::gotoStartOfLine()
{
    LOG("TextDocument::gotoStartOfLine");
    movePosition(QTextCursor::StartOfLine);
}

void TextDocument::gotoEndOfLine()
{
    LOG("TextDocument::gotoEndOfLine");
    movePosition(QTextCursor::EndOfLine);
}

void TextDocument::gotoStartOfWord()
{
    LOG("TextDocument::gotoStartOfWord");
    movePosition(QTextCursor::StartOfWord);
}

void TextDocument::gotoEndOfWord()
{
    LOG("TextDocument::gotoEndOfWord");
    movePosition(QTextCursor::EndOfWord);
}

void TextDocument::gotoNextLine(int count)
{
    LOG("TextDocument::gotoNextLine", count);
    movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, count);
}

void TextDocument::gotoPreviousLine(int count)
{
    LOG("TextDocument::gotoPreviousLine", count);
    movePosition(QTextCursor::Up, QTextCursor::MoveAnchor, count);
}

void TextDocument::gotoPreviousChar(int count)
{
    LOG("TextDocument::gotoPreviousChar", count);
    movePosition(QTextCursor::PreviousCharacter, QTextCursor::MoveAnchor, count);
}

void TextDocument::gotoNextChar(int count)
{
    LOG("TextDocument::gotoNextChar", count);
    movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor, count);
}

void TextDocument::gotoPreviousWord(int count)
{
    LOG("TextDocument::gotoPreviousWord", count);
    movePosition(QTextCursor::PreviousWord, QTextCursor::MoveAnchor, count);
}

void TextDocument::gotoNextWord(int count)
{
    LOG("TextDocument::gotoNextWord", count);
    movePosition(QTextCursor::NextWord, QTextCursor::MoveAnchor, count);
}

void TextDocument::gotoStartOfDocument()
{
    LOG("TextDocument::gotoStartOfDocument");
    movePosition(QTextCursor::Start);
}

void TextDocument::gotoEndOfDocument()
{
    LOG("TextDocument::gotoEndOfDocument");
    movePosition(QTextCursor::End);
}

void TextDocument::unselect()
{
    LOG("TextDocument::unselect");
    QTextCursor cursor = m_document->textCursor();
    cursor.clearSelection();
    m_document->setTextCursor(cursor);
}

void TextDocument::selectAll()
{
    LOG("TextDocument::selectAll");
    m_document->selectAll();
}

void TextDocument::selectStartOfLine()
{
    LOG("TextDocument::selectStartOfLine");
    movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
}

void TextDocument::selectEndOfLine()
{
    LOG("TextDocument::selectEndOfLine");
    movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
}

void TextDocument::selectStartOfWord()
{
    LOG("TextDocument::selectStartOfWord");
    movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);
}

void TextDocument::selectEndOfWord()
{
    LOG("TextDocument::selectEndOfWord");
    movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
}

void TextDocument::selectNextLine(int count)
{
    LOG("TextDocument::selectNextLine", count);
    movePosition(QTextCursor::Down, QTextCursor::KeepAnchor, count);
}

void TextDocument::selectPreviousLine(int count)
{
    LOG("TextDocument::selectPreviousLine", count);
    movePosition(QTextCursor::Up, QTextCursor::KeepAnchor, count);
}

void TextDocument::selectPreviousChar(int count)
{
    LOG("TextDocument::selectPreviousChar", count);
    movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor, count);
}

void TextDocument::selectNextChar(int count)
{
    LOG("TextDocument::selectNextChar", count);
    movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, count);
}

void TextDocument::selectPreviousWord(int count)
{
    LOG("TextDocument::selectPreviousWord", count);
    movePosition(QTextCursor::PreviousWord, QTextCursor::KeepAnchor, count);
}

void TextDocument::selectNextWord(int count)
{
    LOG("TextDocument::selectNextWord", count);
    movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor, count);
}

void TextDocument::remove(int length)
{
    LOG("TextDocument::remove", length);
    QTextCursor cursor = m_document->textCursor();
    cursor.setPosition(cursor.position() + length, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    m_document->setTextCursor(cursor);
}

void TextDocument::insert(const QString &text)
{
    LOG("TextDocument::insert", text);
    m_document->insertPlainText(text);
}

void TextDocument::deleteSelection()
{
    LOG("TextDocument::deleteSelection");
    m_document->textCursor().removeSelectedText();
}

void TextDocument::deleteEndOfLine()
{
    LOG("TextDocument::deleteEndOfLine");
    QTextCursor cursor = m_document->textCursor();
    cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    m_document->setTextCursor(cursor);
}

void TextDocument::deleteStartOfLine()
{
    LOG("TextDocument::deleteStartOfLine");
    QTextCursor cursor = m_document->textCursor();
    cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    m_document->setTextCursor(cursor);
}

void TextDocument::deleteEndOfWord()
{
    LOG("TextDocument::deleteEndOfWord");
    QTextCursor cursor = m_document->textCursor();
    if (!cursor.hasSelection())
        cursor.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    m_document->setTextCursor(cursor);
}

void TextDocument::deleteStartOfWord()
{
    LOG("TextDocument::deleteStartOfWord");
    QTextCursor cursor = m_document->textCursor();
    if (!cursor.hasSelection())
        cursor.movePosition(QTextCursor::PreviousWord, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    m_document->setTextCursor(cursor);
}

void TextDocument::deletePreviousCharacter(int count)
{
    LOG("TextDocument::deletePreviousCharacter", count);
    QTextCursor cursor = m_document->textCursor();
    cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor, count);
    cursor.removeSelectedText();
    m_document->setTextCursor(cursor);
}

void TextDocument::deleteNextCharacter(int count)
{
    LOG("TextDocument::deleteNextCharacter", count);
    QTextCursor cursor = m_document->textCursor();
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, count);
    cursor.removeSelectedText();
    m_document->setTextCursor(cursor);
}

bool TextDocument::find(const QString &text)
{
    LOG("TextDocument::find", text);
    return m_document->find(text);
}

void TextDocument::foo()
{
    LOG("TextDocument::foo");
}

void TextDocument::bar()
{
    LOG("TextDocument::bar");
    foo();
}

bool TextDocument::eventFilter(QObject *watched, QEvent *event)
{
    Q_ASSERT(watched == m_document);

    if (event->type() == QEvent::KeyPress) {
        auto keyEvent = static_cast<QKeyEvent *>(event);

        if (keyEvent == QKeySequence::MoveToNextChar)
            gotoNextChar();
        else if (keyEvent == QKeySequence::MoveToPreviousChar)
            gotoPreviousChar();
        else if (keyEvent == QKeySequence::SelectNextChar)
            selectNextChar();
        else if (keyEvent == QKeySequence::SelectPreviousChar)
            selectPreviousChar();
        else if (keyEvent == QKeySequence::SelectNextWord)
            selectNextWord();
        else if (keyEvent == QKeySequence::SelectPreviousWord)
            selectPreviousWord();
        else if (keyEvent == QKeySequence::SelectStartOfLine)
            selectStartOfLine();
        else if (keyEvent == QKeySequence::SelectEndOfLine)
            selectEndOfLine();
        else if (keyEvent == QKeySequence::SelectPreviousLine)
            selectPreviousLine();
        else if (keyEvent == QKeySequence::SelectNextLine)
            selectNextLine();
        else if (keyEvent == QKeySequence::MoveToNextWord)
            gotoNextWord();
        else if (keyEvent == QKeySequence::MoveToPreviousWord)
            gotoPreviousWord();
        else if (keyEvent == QKeySequence::MoveToNextLine)
            gotoNextLine();
        else if (keyEvent == QKeySequence::MoveToPreviousLine)
            gotoPreviousLine();
        else if (keyEvent == QKeySequence::MoveToStartOfLine)
            gotoStartOfLine();
        else if (keyEvent == QKeySequence::MoveToEndOfLine)
            gotoEndOfLine();
        else if (keyEvent == QKeySequence::MoveToStartOfDocument)
            gotoStartOfDocument();
        else if (keyEvent == QKeySequence::MoveToEndOfDocument)
            gotoEndOfDocument();
        else if (keyEvent == QKeySequence::MoveToNextPage)
            return false;
        else if (keyEvent == QKeySequence::MoveToPreviousPage)
            return false;
        else if (keyEvent == QKeySequence::Delete)
            m_document->textCursor().hasSelection() ? deleteSelection() : deleteNextCharacter();
        else if (keyEvent == QKeySequence::Backspace
                 || (keyEvent->key() == Qt::Key_Backspace
                     && !(keyEvent->modifiers() & ~Qt::ShiftModifier))) // test is coming from QTextWidgetControl
            m_document->textCursor().hasSelection() ? deleteSelection() : deletePreviousCharacter();
        else if (keyEvent == QKeySequence::InsertParagraphSeparator)
            insert("\n");
        else if (keyEvent == QKeySequence::InsertLineSeparator)
            insert(QString(QChar::LineSeparator));
        else if (keyEvent == QKeySequence::DeleteEndOfWord)
            deleteEndOfWord();
        else if (keyEvent == QKeySequence::DeleteStartOfWord)
            deleteStartOfWord();
        else if (keyEvent == QKeySequence::DeleteEndOfLine)
            deleteEndOfLine();
        else if (keyEvent == QKeySequence::SelectAll)
            selectAll();
        else if (!keyEvent->text().isEmpty()) {
            auto control = m_document->findChild<QWidgetTextControl *>();
            if (control->isAcceptableInput(keyEvent))
                insert(keyEvent->text());
        }

        return true;
    }
    return QObject::eventFilter(watched, event);
}
