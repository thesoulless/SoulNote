/*********************************************************************************
**   SoulNote - Simple Text Editor                                              **
**   Copyright (C) 2009  Hamed Nemati Ziabari   <hitech.innovative@gmail.com>   **
**                                                                              **
**   This program is free software: you can redistribute it and/or modify       **
**   it under the terms of the GNU General Public License as published by       **
**   the Free Software Foundation, either version 3 of the License, or          **
**   (at your option) any later version.                                        **
**                                                                              **
**   This program is distributed in the hope that it will be useful,            **
**   but WITHOUT ANY WARRANTY; without even the implied warranty of             **
**   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          **
**   Lesser General Public License for more details.                            **
**                                                                              **
**   You should have received a copy of the GNU General Public License          **
**   along with this program.  If not, see <http://www.gnu.org/licenses/>.      **
**   the Free Software Foundation, Inc.,                                        **
**   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA               **
*********************************************************************************/

#include <QtGui>
#include <QtCore/QDebug>
#include "findwidget.h"
#include "textedit.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <fstream>

TextEdit::TextEdit(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);
    m_lastFindString = QString();
    //setStyleSheet("QPlainTextEdit {color:#8fd2de; background: #2e3436;}"); //background

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    //connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);

    /** \todo Replace tab with spaces! */
    int charWidth = QFontMetrics(font()).width(QChar(' '));
    setTabStopWidth(charWidth * 8);
}

TextEdit::~TextEdit(){}

int TextEdit::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

void TextEdit::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    if (lineNumberArea->isVisiblePrivate())
        setViewportMargins(lineNumberAreaWidth() + 12, 0, 0, 0);
    else
        setViewportMargins(0, 0, 0, 0);
}

void TextEdit::setLineNumberAreaVisible(bool show)
{
    lineNumberArea->setVisible(show);
    lineNumberArea->setVisiblePrivate(show);
    updateLineNumberAreaWidth(0);
}

void TextEdit::updateLineNumberArea(const QRect &rect, int dy)
{    
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width() + 12, rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);    
}

void TextEdit::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth() + 12, cr.height()));
}

void TextEdit::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;
        QPalette a;

        selection.format.setBackground(a.brush(QPalette::Window));
        //selection.format.setProperty(QTextFormat::BlockNonBreakableLines, false);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        selection.cursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::MoveAnchor);
        selection.cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void TextEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    QPalette b;
    painter.fillRect(event->rect(), b.brush(QWidget::backgroundRole()));


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::lightGray);
            painter.drawText(0, top, lineNumberArea->width() - 4, fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void TextEdit::gotoLine(int line, int column)
{
    const int blockNumber = line - 1;
    const QTextBlock &block = document()->findBlockByNumber(blockNumber);
    if (block.isValid()) {
        QTextCursor cursor(block);
        if (column > 0) {
            if (block.text() != "" && column < block.text().length())
                cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, column);
            else
                cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::MoveAnchor);
        } else {
            int pos = cursor.position();
            while (document()->characterAt(pos).category() == QChar::Separator_Space) {
                ++pos;
            }
            cursor.setPosition(pos);
        }
        setTextCursor(cursor);
        centerCursor();
    }
}

void TextEdit::findExpretion(const QString &findString, QTextDocument::FindFlags options)
{
    highlight(findString, options);

    QTextDocument *document = this->document();
    QTextCursor findCursor(document);
    QTextCursor cursor(document);

    int pos = this->textCursor().position();
    if (textCursor().hasSelection() && options == QTextDocument::FindBackward){
        findCursor.setPosition(pos, QTextCursor::MoveAnchor);
        findCursor.movePosition(QTextCursor::StartOfWord, QTextCursor::MoveAnchor);
    } else {
        findCursor.setPosition(pos, QTextCursor::MoveAnchor);
    }
    if (textCursor().hasSelection() && options == QTextDocument::FindBackward){
        cursor.setPosition(pos, QTextCursor::MoveAnchor);
        cursor.movePosition(QTextCursor::StartOfWord, QTextCursor::MoveAnchor);
    } else {
        cursor.setPosition(pos, QTextCursor::MoveAnchor);
    }

    if ((m_lastFindString.contains(findString) || findString.contains(m_lastFindString)) &&
        options != QTextDocument::FindBackward && textCursor().hasSelection() &&
        findString != m_lastFindString){
        findCursor.movePosition(QTextCursor::StartOfWord, QTextCursor::MoveAnchor);
    }

    //if (!(this->textCursor().hasSelection()) && (!findString.isEmpty() || !findString.isNull()))
    findCursor = document->find(findString, findCursor, options);

    if (findCursor.isNull()) {
        if (options == QTextDocument::FindBackward){
            findCursor = cursor;
            findCursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        }
        findCursor = document->find(findString, findCursor, options);
    }

    if (findCursor.isNull()) {
        this->setTextCursor(cursor);
    } else {
        this->setTextCursor(findCursor);
        //m_isFirstTime = false;
    }
    m_lastFindString = findString;
}

void TextEdit::highlight(const QString &findString, QTextDocument::FindFlags options)
{
    clearFind();
    QTextDocument *document = this->document();
    QTextCursor highlightCursor(document);
    QTextCharFormat colorFormat(highlightCursor.charFormat());
    colorFormat.setForeground(Qt::white);
    QPalette a;

    QBrush back(a.brush(QPalette::Window).color().darker(120));
    colorFormat.setBackground(back);

    while (!highlightCursor.isNull() && !highlightCursor.atEnd()) {
        highlightCursor = document->find(findString, highlightCursor, options);

        if (!highlightCursor.isNull()) {
            highlightCursor.setCharFormat(colorFormat);
        }
    }
}

void TextEdit::replaceExpretion(const QString &replaceString){
    QTextCursor qc = this->textCursor();
    if (qc.hasSelection())
        qc.insertText(replaceString);
}

void TextEdit::clearFind()
{
    QTextDocument *document = this->document();
    QTextCursor highlightCursor(document);
    QTextCharFormat plainFormat(highlightCursor.charFormat());
    highlightCursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
    highlightCursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    highlightCursor.setCharFormat(plainFormat);
}
