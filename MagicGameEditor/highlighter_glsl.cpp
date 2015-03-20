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

#include "highlighter_glsl.h"

HighlighterGLSL::HighlighterGLSL(QTextDocument *parent) : Highlighter(parent)
{
    HighlightingRule rule;

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\s*\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\band\\b" << "\\bbreak\\b" << "\\bdo\\b"
                    << "\\belse\\b" << "\\belseif\\b" << "\\bend\\b"
                    << "\\bfalse\\b" << "\\bfor\\b" << "\\bfunction\\b"
                    << "\\bif\\b" << "\\bin\\b" << "\\blocal\\b"
                    << "\\bnil\\b" << "\\bnot\\b" << "\\bor\\b"
                    << "\\brepeat\\b" << "\\breturn\\b" << "\\bthen\\b"
                    << "\\btrue\\b" << "\\buntil\\b" << "\\bwhile\\b" << "\\bdiscard\\b"
                    << "\\bmagic3d\\b" << "\\bthis\\b" << "\\bstruct\\b";

    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    typeFormat.setForeground(Qt::darkYellow);
    typeFormat.setFontWeight(QFont::Bold);
    QStringList typesPatterns;
    typesPatterns << "\\bvoid\\b" << "\\bbool\\b" << "\\bint\\b" << "\\bfloat\\b"
                    << "\\bvec2\\b" << "\\bvec3\\b" << "\\bvec4\\b"
                    << "\\bbvec2\\b" << "\\bbvec3\\b" << "\\bbvec4\\b"
                    << "\\bivec2\\b" << "\\bivec3\\b" << "\\bivec4\\b"
                    << "\\bmat2\\b" << "\\bmat3\\b" << "\\bmat4\\b"
                    << "\\bsampler2D\\b" << "\\bsamplerCube\\b" << "\\bprecision\\b"
                    << "\\bin\\b" << "\\bout\\b" << "\\binout\\b";

    foreach (const QString &pattern, typesPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = typeFormat;
        highlightingRules.append(rule);
    }

    storageFormat.setForeground(Qt::darkCyan);
    storageFormat.setFontWeight(QFont::Bold);
    QStringList storagePatterns;
    storagePatterns << "\\bnone\\b" << "\\bconst\\b" << "\\battribute\\b" << "\\buniform\\b" << "\\bvarying\\b";

    foreach (const QString &pattern, storagePatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = storageFormat;
        highlightingRules.append(rule);
    }

    precisionFormat.setForeground(Qt::red);
    //precisionFormat.setFontWeight(QFont::Bold);
    QStringList precisionPatterns;
    precisionPatterns << "\\bhighp\\b" << "\\bmediump\\b" << "\\blowp\\b";

    foreach (const QString &pattern, precisionPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = precisionFormat;
        highlightingRules.append(rule);
    }

    defineFormat.setForeground(Qt::darkGreen);
    //precisionFormat.setFontWeight(QFont::Bold);
    QStringList definePatterns;
    definePatterns << "#\\b" << "#define\\b" << "#undef\\b"
                   << "#if\\b" << "#ifdef\\b" << "#ifndef\\b"
                   << "#else\\b" << "#elif\\b" << "#endif\\b"
                   << "#error\\b" << "#pragma\\b" << "#extension\\b"
                   << "#version\\b" << "#line\\b"
                   << "__LINE__\\b" << "__FILE__\\b" << "__VERSION__\\b"
                   << "GL_ES\\b" << "GL_FRAGMENT_PRECISION_HIGH\\b"
                   << "#[^\n]*";

    foreach (const QString &pattern, definePatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = defineFormat;
        highlightingRules.append(rule);
    }

    objectFormat.setForeground(Qt::darkRed);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\s*(?:\\:|\\.))");
    rule.format = objectFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegExp("\\b\\.[A-Za-z0-9_]+");
    rule.format = objectFormat;
    highlightingRules.append(rule);

    varFormat.setForeground(Qt::darkRed);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\s*=)");
    rule.format = varFormat;
    highlightingRules.append(rule);

    punctuationFormat.setForeground(Qt::black);
    rule.pattern = QRegExp("\\b\\.\\b");
    rule.format = punctuationFormat;
    highlightingRules.append(rule);

    quotationFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("\"[^\"]*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::darkGray);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::darkGray);

    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
}

void HighlighterGLSL::highlightBlock(const QString &text)
{
    Highlighter::highlightBlock(text);

    bool comment = false;
    QList<HighlightQuotation> quotationList;
    int index = 0;

    if (previousBlockState() != 1)
    {
        foreach (const HighlightingRule &rule, highlightingRules)
        {
            QRegExp expression(rule.pattern);
            index = expression.indexIn(text);

            while (index >= 0)
            {
                bool formatBlock = true;
                int length = expression.matchedLength();
                comment = rule.format.foreground() == Qt::darkGray;
                if ( rule.format.foreground() == Qt::darkMagenta )
                {
                    quotationList << HighlightQuotation(index, index + length);
                }

                if (comment)
                {
                    foreach(HighlightQuotation quotes, quotationList)
                    {
                        if (index > quotes.start_index && index < quotes.end_index)
                        {
                            formatBlock = false;
                            break;
                        }
                    }
                }

                if (formatBlock)
                {
                    setFormat(index, length, rule.format);
                }

                if (comment && formatBlock)
                {
                    break;
                }
                else
                {
                    if (comment)
                    {
                        index = expression.indexIn(text, index + 2);
                    }
                    else
                    {
                        index = expression.indexIn(text, index + length);
                    }
                }
            }

            if (comment)
            {
                break;
            }
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

    if (!comment || (comment && startIndex >= 0 && startIndex <= index))
    {
        while (startIndex >= 0)
        {
            int endIndex = commentEndExpression.indexIn(text, startIndex + 2);

            int commentLength = 1;

            if (endIndex == -1)
            {
                setCurrentBlockState(1);
                commentLength = text.length() - startIndex;
            }
            else
            {
                commentLength = endIndex - startIndex + commentEndExpression.matchedLength();
            }
            setFormat(startIndex, commentLength, multiLineCommentFormat);
            startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
        }
    }
}
