#pragma once

#include <QObject>
#include <QPointer>
#include <QTextCursor>
#include <QQmlEngine>

class QPlainTextEdit;

class TextDocument : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QString currentWord READ currentWord NOTIFY positionChanged)
    Q_PROPERTY(QString selectedText READ selectedText NOTIFY selectionChanged)

public:
    TextDocument(QPlainTextEdit *textEdit, QObject *parent = nullptr);
    ~TextDocument();

    static TextDocument *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine);

    QString currentWord() const;
    QString selectedText() const;
    bool hasSelection() const;

signals:
    void positionChanged();
    void selectionChanged();

public slots:
    void gotoStartOfLine();
    void gotoEndOfLine();
    void gotoStartOfWord();
    void gotoEndOfWord();
    void gotoPreviousLine(int count = 1);
    void gotoNextLine(int count = 1);
    void gotoPreviousChar(int count = 1);
    void gotoNextChar(int count = 1);
    void gotoPreviousWord(int count = 1);
    void gotoNextWord(int count = 1);
    void gotoStartOfDocument();
    void gotoEndOfDocument();

    void unselect();
    void selectAll();

    void selectStartOfLine();
    void selectEndOfLine();
    void selectStartOfWord();
    void selectEndOfWord();
    void selectPreviousLine(int count = 1);
    void selectNextLine(int count = 1);
    void selectPreviousChar(int count = 1);
    void selectNextChar(int count = 1);
    void selectPreviousWord(int count = 1);
    void selectNextWord(int count = 1);

    void remove(int length);
    void insert(const QString &text);

    void deleteSelection();
    void deleteEndOfLine();
    void deleteStartOfLine();
    void deleteEndOfWord();
    void deleteStartOfWord();
    void deletePreviousCharacter(int count = 1);
    void deleteNextCharacter(int count = 1);

    bool find(const QString &text);

private:
    void movePosition(QTextCursor::MoveOperation operation, QTextCursor::MoveMode mode = QTextCursor::MoveAnchor,
                      int count = 1);

    QPointer<QPlainTextEdit> m_document;
    inline static TextDocument *m_instance;
};
