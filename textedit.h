/****************************************************************************
**   <one line to give the program's name and a brief idea of              **
**   what it does.>                                                        **
**   Copyright (C) 2009  Hamed Nemati Ziabari                              **
**                                                                         **
**   This program is free software: you can redistribute it and/or modify  **
**   it under the terms of the GNU General Public License as published by  **
**   the Free Software Foundation, either version 3 of the License, or     **
**   (at your option) any later version.                                   **
**                                                                         **
**   This program is distributed in the hope that it will be useful,       **
**   but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     **
**   Lesser General Public License for more details.                       **
**                                                                         **
**   You should have received a copy of the GNU General Public License     **
**   along with this program.  If not, see <http://www.gnu.org/licenses/>. **
**   the Free Software Foundation, Inc.,                                   **
**   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA          **
****************************************************************************/

#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QtGui/QPlainTextEdit>
#include <QtGui/QCompleter>
#include <QStringListModel>
#include <QObject>

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE

class LineNumberArea;

class TextEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    TextEdit(QWidget *parent = 0);
    virtual ~TextEdit();


    void lineNumberAreaPaintEvent(QPaintEvent *event);    
    int lineNumberAreaWidth();
    void setLineNumberAreaVisible(bool show);

    void gotoLine(int line, int column = 0);

//    void setCompleter(QCompleter *c);
//    QCompleter *completer() const;

protected:
    void resizeEvent(QResizeEvent *event);
//    void keyPressEvent(QKeyEvent *e);
//    void focusInEvent(QFocusEvent *e);

public slots:
    void findExpretion(const QString&, QTextDocument::FindFlags options = 0);
    void highlight(const QString&, QTextDocument::FindFlags options = 0);
//    void findNext();
    void replaceExpretion(const QString&);
    void clearFind();

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);
//    void insertCompletion(const QString &completion);

private:
    LineNumberArea *lineNumberArea;
    bool m_isFirstTime;
    QString m_searchString;
    QString m_lastFindString;
//    QString textUnderCursor() const;
//    QAbstractItemModel *modelFromDic(const QString& incorrect, int size);
//    QCompleter *c;
};



//![extraarea]

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(TextEdit *editor) : QWidget(editor) {
        m_editor = editor;
        m_visible = true;
    }

    QSize sizeHint() const {
        return QSize(m_editor->lineNumberAreaWidth(), 0);
    }
    void setVisiblePrivate(bool show) { m_visible = show; }
    bool isVisiblePrivate() const { return m_visible; }

protected:
    void paintEvent(QPaintEvent *event) {
        m_editor->lineNumberAreaPaintEvent(event);
    }

private:
    TextEdit *m_editor;
    bool m_visible;
};

//![extraarea]

//![extraarea]
/*
class LineNumberArea : public QWidget
{
public:
    LineNumberArea(TextEdit *editor) : QWidget(editor) {
        m_editor = editor;
    }

    QSize sizeHint() const {
        return QSize(m_editor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) {
        m_editor->lineNumberAreaPaintEvent(event);
    }

private:
    TextEdit *m_editor;
};

//![extraarea]*/

#endif // TEXTEDIT_H
