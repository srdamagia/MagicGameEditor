/******************************************************************************
 @Copyright    Copyright (C) 2008 - 2015 by MagicTech.

 @Platform     ANSI compatible
******************************************************************************/
/*
Magic3D Engine
Copyright (c) 2008-2015
Thiago C. Moraes
http://www.magictech.com.br

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software.
   If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include <QtGui>

#include "codeeditor.h"
#include "textfind.h"


CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    startLine = 0;
    lineNumberArea = new LineNumberArea(this);
    find = NULL;

    fileName = "";

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(matchAll()));

    setLineWrapMode(NoWrap);
    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

void CodeEditor::setTextFind(TextFind* find)
{
    this->find = find;
}

TextFind* CodeEditor::getTextFind()
{
    return find;
}

void CodeEditor::setCursorPosition(int pos)
{
    QTextCursor cursor = textCursor();
    cursor.setPosition(pos);
    setTextCursor(cursor);
}

int CodeEditor::getCursorPosition()
{
    return textCursor().position();
}

void CodeEditor::setFileName(const QString& fileName)
{
    this->fileName = fileName;
}

const QString& CodeEditor::getFileName()
{
    return fileName;
}

void CodeEditor::setStartLine(int startLine)
{
    this->startLine = startLine;
}

int CodeEditor::getLineCount()
{
    return blockCount();
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }

    if (digits < 6)
    {
        digits = 6;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int newBlockCount)
{
    if (newBlockCount)
    {
    }
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}



void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}



void CodeEditor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Tab || event->key() == Qt::Key_Backtab)
    {
        QString newTab = "    ";
        int tabSize = 4;

        QTextCursor cursor = textCursor();
        int start = cursor.selectionStart();
        int end = cursor.selectionEnd();

        QTextBlock block = cursor.block();

        bool increment = block.contains(start);

        cursor.beginEditBlock();
        while (block.isValid())
        {
            if (block.contains(start) && block.contains(end) && start == end)
            {
                if (event->key() == Qt::Key_Backtab)
                {
                    int pos = cursor.positionInBlock() - 1;
                    QString blockText = block.text();

                    for (int i = 0; i < tabSize; i++)
                    {
                        int newPos = pos - i;
                        if (newPos >= 0 && blockText.size() > pos && blockText.at(newPos) == ' ')
                        {
                            cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
                        }
                        else
                        {
                            break;
                        }
                    }

                    cursor.removeSelectedText();
                }
                else
                {
                    cursor.insertText(newTab);
                }
            }
            else
            {
                if (event->key() == Qt::Key_Backtab)
                {
                    int pos = block.position();
                    cursor.setPosition(pos, QTextCursor::MoveAnchor);
                    QString blockText = block.text();

                    for (int i = 0; i < tabSize; i++)
                    {
                        if (blockText.size() > i && blockText.at(i) == ' ')
                        {
                            cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
                        }
                        else
                        {
                            break;
                        }
                    }

                    cursor.removeSelectedText();
                }
                else
                {
                    int pos = block.position();
                    cursor.setPosition(pos, QTextCursor::MoveAnchor);
                    cursor.insertText(newTab);
                }
            }

            cursor = textCursor();
            start = cursor.selectionStart();
            end = cursor.selectionEnd();
            if (block.contains(increment ? end : start))
            {
                break;
            }
            block = (increment ? block.next() : block.previous());
        }
        cursor.endEditBlock();
        event->accept();
    }
    else if(event->key() == Qt::Key_F3)
    {
        if (find)
        {
            if (find->isEmpty())
            {
                find->focusFind();
            }
            else
            {
                if (event->modifiers() & Qt::ShiftModifier)
                {
                    find->findPrevious();
                }
                else
                {
                    find->findNext();
                }
            }
        }
    }
    else if(event->key() == Qt::Key_F && event->modifiers() & Qt::ControlModifier)
    {
        if (find)
        {
            find->focusFind();
        }
    }
    else
    {
        QPlainTextEdit::keyPressEvent(event);
    }
}

void CodeEditor::focusInEvent(QFocusEvent * event)
{
    QPlainTextEdit::focusInEvent(event);
    if (find)
    {
        find->setFindEditor(this);
    }
}

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;

        //QColor lineColor = QColor(224, 240, 255);
        QColor lineColor = QColor(Qt::lightGray);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(startLine + blockNumber + 1);
            painter.setPen(Qt::darkGray);
            painter.drawText(0, top, lineNumberArea->width() - fontMetrics().width(QLatin1Char('9')), fontMetrics().height(),
                          Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void CodeEditor::matchAll()
{
    selections = extraSelections();
    matchParentheses();
    matchBrackets();
    setExtraSelections(selections);
}

// walk through and check that we don't exceed 80 chars per line
void CodeEditor::matchParentheses()
{
    //bool match = false;
    QList<QTextEdit::ExtraSelection> selections;
    setExtraSelections(selections);

    TextBlockData *data = static_cast<TextBlockData *>(textCursor().block().userData());

    if (data)
    {
        QVector<PositionInfo *> infos = data->parentheses();

        int pos = textCursor().block().position();
        for (int i = 0; i < infos.size(); ++i)
        {
            PositionInfo *info = infos.at(i);

            int curPos = textCursor().position() - textCursor().block().position();
            if (info->position == curPos - 1 && info->character == '(')
            {
                if (matchLeftParenthesis(textCursor().block(), i + 1, 0))
                    createParenthesisSelection(pos + info->position);
            }
            else if (info->position == curPos - 1 && info->character == ')')
            {
                if (matchRightParenthesis(textCursor().block(), i - 1, 0))
                    createParenthesisSelection(pos + info->position);
            }
        }
    }
}

bool CodeEditor::matchLeftParenthesis(QTextBlock currentBlock, int index, int numRightParentheses)
{
    TextBlockData *data = static_cast<TextBlockData *>(currentBlock.userData());
    QVector<PositionInfo *> infos = data->parentheses();

    int docPos = currentBlock.position();
    for (; index < infos.size(); ++index)
    {
        PositionInfo *info = infos.at(index);

        if (info->character == '(')
        {
            ++numRightParentheses;
            continue;
        }

        if (info->character == ')' && numRightParentheses == 0)
        {
            createParenthesisSelection(docPos + info->position);
            return true;
        }
        else
            --numRightParentheses;
    }

    currentBlock = currentBlock.next();
    if (currentBlock.isValid())
        return matchLeftParenthesis(currentBlock, 0, numRightParentheses);

    return false;
}

bool CodeEditor::matchRightParenthesis(QTextBlock currentBlock, int index, int numLeftParentheses)
{
    TextBlockData *data = static_cast<TextBlockData *>(currentBlock.userData());
    QVector<PositionInfo *> parentheses = data->parentheses();

    int docPos = currentBlock.position();
    for (; index > -1 && parentheses.size() > 0; --index)
    {
        PositionInfo *info = parentheses.at(index);
        if (info->character == ')')
        {
            ++numLeftParentheses;
            continue;
        }
        if (info->character == '(' && numLeftParentheses == 0)
        {
            createParenthesisSelection(docPos + info->position);
            return true;
        } else
            --numLeftParentheses;
    }

    currentBlock = currentBlock.previous();
    if (currentBlock.isValid())
        return matchRightParenthesis(currentBlock, 0, numLeftParentheses);

    return false;
}

void CodeEditor::createParenthesisSelection(int pos)
{
    QTextEdit::ExtraSelection selection;
    QTextCharFormat format = selection.format;
    format.setBackground(Qt::green);
    selection.format = format;

    QTextCursor cursor = textCursor();
    cursor.setPosition(pos);
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
    selection.cursor = cursor;

    selections.append(selection);
}

void CodeEditor::matchBrackets()
{
    //bool match = false;
    QList<QTextEdit::ExtraSelection> selections;
    setExtraSelections(selections);

    TextBlockData *data = static_cast<TextBlockData *>(textCursor().block().userData());

    if (data)
    {
        QVector<PositionInfo *> infos = data->brackets();

        int pos = textCursor().block().position();
        for (int i = 0; i < infos.size(); ++i)
        {
            PositionInfo *info = infos.at(i);

            int curPos = textCursor().position() - textCursor().block().position();
            if (info->position == curPos - 1 && info->character == '{')
            {
                if (matchLeftBrackets(textCursor().block(), i + 1, 0))
                    createBracketsSelection(pos + info->position);
            }
            else if (info->position == curPos - 1 && info->character == '}')
            {
                if (matchRightBrackets(textCursor().block(), i - 1, 0))
                    createBracketsSelection(pos + info->position);
            }
        }
    }
}

bool CodeEditor::matchLeftBrackets(QTextBlock currentBlock, int index, int numRightBrackets)
{
    TextBlockData *data = static_cast<TextBlockData *>(currentBlock.userData());
    QVector<PositionInfo *> infos = data->brackets();

    int docPos = currentBlock.position();
    for (; index < infos.size(); ++index)
    {
        PositionInfo *info = infos.at(index);

        if (info->character == '{')
        {
            ++numRightBrackets;
            continue;
        }

        if (info->character == '}' && numRightBrackets == 0)
        {
            createBracketsSelection(docPos + info->position);
            return true;
        }
        else
            --numRightBrackets;
    }

    currentBlock = currentBlock.next();
    if (currentBlock.isValid())
        return matchLeftBrackets(currentBlock, 0, numRightBrackets);

    return false;
}

bool CodeEditor::matchRightBrackets(QTextBlock currentBlock, int index, int numLeftBrackets)
{
    TextBlockData *data = static_cast<TextBlockData *>(currentBlock.userData());
    QVector<PositionInfo *> brackets = data->brackets();

    int docPos = currentBlock.position();
    for (; index > -1 && brackets.size() > 0; --index)
    {
        PositionInfo *info = brackets.at(index);
        if (info->character == '}')
        {
            ++numLeftBrackets;
            continue;
        }
        if (info->character == '{' && numLeftBrackets == 0)
        {
            createBracketsSelection(docPos + info->position);
            return true;
        } else
            --numLeftBrackets;
    }

    currentBlock = currentBlock.previous();
    if (currentBlock.isValid())
        return matchRightBrackets(currentBlock, 0, numLeftBrackets);

    return false;
}

void CodeEditor::createBracketsSelection(int pos)
{
    QTextEdit::ExtraSelection selection;
    QTextCharFormat format = selection.format;
    format.setBackground(Qt::green);
    selection.format = format;

    QTextCursor cursor = textCursor();
    cursor.setPosition(pos);
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
    selection.cursor = cursor;

    selections.append(selection);
}

