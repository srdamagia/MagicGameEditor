/******************************************************************************
 @Copyright    Copyright (C) 2008 - 2016 by MagicTech.

 @Platform     ANSI compatible
******************************************************************************/
/*
Magic3D Engine
Copyright (c) 2008-2016
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

#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QObject>

#include "highlighter.h"

 class QPaintEvent;
 class QResizeEvent;
 class QSize;
 class QWidget;

 class LineNumberArea;
 class TextFind;

 class CodeEditor : public QPlainTextEdit
 {
     Q_OBJECT

 public:
     explicit CodeEditor(QWidget *parent = 0);

     void lineNumberAreaPaintEvent(QPaintEvent *event);
     int lineNumberAreaWidth();

     void setStartLine(int startLine);
     int getLineCount();

     void setTextFind(TextFind* find);
     TextFind* getTextFind();

     void setCursorPosition(int pos);
     int getCursorPosition();

     void setFileName(const QString& fileName);
     const QString& getFileName();

 protected:
     void resizeEvent(QResizeEvent *event);
     void keyPressEvent(QKeyEvent* event);

     void focusInEvent(QFocusEvent * event);

 private slots:
     void updateLineNumberAreaWidth(int newBlockCount);
     void highlightCurrentLine();
     void updateLineNumberArea(const QRect &, int);
     void matchParentheses();
     void matchBrackets();

     void matchAll();

 private:
     int startLine;
     QWidget *lineNumberArea;

     QList<QTextEdit::ExtraSelection> selections;

     TextFind* find;

     QString fileName;

     bool matchLeftParenthesis(QTextBlock currentBlock, int index, int numRightParentheses);
     bool matchRightParenthesis(QTextBlock currentBlock, int index, int numLeftParentheses);
     void createParenthesisSelection(int pos);

     bool matchLeftBrackets(QTextBlock currentBlock, int index, int numRightBrackets);
     bool matchRightBrackets(QTextBlock currentBlock, int index, int numLeftBrackets);
     void createBracketsSelection(int pos);
 };


 class LineNumberArea : public QWidget
 {
 public:
     explicit LineNumberArea(CodeEditor *editor) : QWidget(editor)
     {
         codeEditor = editor;
     }

     QSize sizeHint() const {
         return QSize(codeEditor->lineNumberAreaWidth(), 0);
     }

 protected:
     void paintEvent(QPaintEvent *event) {
         codeEditor->lineNumberAreaPaintEvent(event);
     }

 private:
     CodeEditor *codeEditor;
 };


 #endif
