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

#include <QtGui>

#include "highlighter.h"

TextBlockData::TextBlockData()
{
    // Nothing to do
}

QVector<PositionInfo *> TextBlockData::parentheses()
{
    return m_parentheses;
}

QVector<PositionInfo *> TextBlockData::brackets()
{
    return m_brackets;
}


void TextBlockData::insertParentheses(PositionInfo *info)
{
    int i = 0;
    while (i < m_parentheses.size() && info->position > m_parentheses.at(i)->position)
        ++i;

    m_parentheses.insert(i, info);
}

void TextBlockData::insertBrackets(PositionInfo *info)
{
    int i = 0;
    while (i < m_brackets.size() && info->position > m_brackets.at(i)->position)
        ++i;

    m_brackets.insert(i, info);
}

Highlighter::Highlighter(QTextDocument *document)
    : QSyntaxHighlighter(document)
{
}

void Highlighter::highlightBlock(const QString &text)
{
    TextBlockData *data = new TextBlockData;
    highlightBlockParentheses(data, text);
    highlightBlockBrackets(data, text);
    setCurrentBlockUserData(data);
}

void Highlighter::highlightBlockParentheses(TextBlockData* data, const QString &text)
{
    int leftPos = text.indexOf('(');
    while (leftPos != -1) {
        PositionInfo *info = new PositionInfo;
        info->character = '(';
        info->position = leftPos;

        data->insertParentheses(info);
        leftPos = text.indexOf('(', leftPos + 1);
    }

    int rightPos = text.indexOf(')');
    while (rightPos != -1) {
        PositionInfo *info = new PositionInfo;
        info->character = ')';
        info->position = rightPos;

        data->insertParentheses(info);

        rightPos = text.indexOf(')', rightPos +1);
    }
}

void Highlighter::highlightBlockBrackets(TextBlockData* data, const QString &text)
{
    int leftPos = text.indexOf('{');
    while (leftPos != -1) {
        PositionInfo *info = new PositionInfo;
        info->character = '{';
        info->position = leftPos;

        data->insertBrackets(info);
        leftPos = text.indexOf('{', leftPos + 1);
    }

    int rightPos = text.indexOf('}');
    while (rightPos != -1) {
        PositionInfo *info = new PositionInfo;
        info->character = '}';
        info->position = rightPos;

        data->insertBrackets(info);

        rightPos = text.indexOf('}', rightPos +1);
    }
}
