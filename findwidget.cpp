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

#include <QtCore/QtDebug>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>

#include "findwidget.h"
#include "textedit.h"

FindWidget::FindWidget(TextEdit *parent) :
        QWidget(parent)
{
    parent ? m_textEdit = parent : m_textEdit = 0;
    m_findFlags = 0;
    setWindowOpacity(0.7);

    m_findLabel = new QLabel(tr("Find:"), this);
    m_replaceLabel = new QLabel(tr("Replace with:"), this);
    m_findLineEdit = new QLineEdit(this);
    m_replaceLineEdit = new QLineEdit(this);    
    m_findPrevious = new QPushButton(tr("<"), this);
    m_findNext = new QPushButton(tr(">"), this);
    m_replacePrevious = new QPushButton(tr("<"), this);
    m_replaceNext = new QPushButton(tr(">"), this);
    m_replaceAll = new QPushButton(tr("Replace All"), this);
    m_close = new QPushButton(tr(""), this);
    m_close->setFlat(true);
    //    m_close->
    QStyle *style = this->style();
    QIcon closeIcon = style->standardIcon(QStyle::SP_DialogCloseButton);
    m_close->setIcon(closeIcon);
    m_close->setStyleSheet("margin:0px; padding:0px; border: none;");
    m_close->setIconSize(QSize(20, 20));

    m_close->setMaximumSize(22, 22);
    m_findNext->setMaximumSize(25, 25);
    m_findPrevious->setMaximumSize(25, 25);
    m_replaceNext->setMaximumSize(25, 25);
    m_replacePrevious->setMaximumSize(25, 25);
    m_findNext->setFlat(true);
    m_findNext->setFocusPolicy(Qt::NoFocus);
    m_findPrevious->setFlat(true);
    m_findPrevious->setFocusPolicy(Qt::NoFocus);
    m_replaceNext->setFlat(true);
    m_replaceNext->setFocusPolicy(Qt::NoFocus);
    m_replacePrevious->setFlat(true);
    m_replacePrevious->setFocusPolicy(Qt::NoFocus);
    m_replaceAll->setFlat(true);
    m_findLineEdit->setMaximumWidth(500);
    m_findLineEdit->setMinimumWidth(100);
    m_findLineEdit->installEventFilter(this);
    m_replaceLineEdit->setMaximumWidth(500);
    m_replaceLineEdit->setMinimumWidth(100);
    m_replaceLineEdit->installEventFilter(this);
    m_findNext->installEventFilter(this);
    m_findPrevious->installEventFilter(this);
    m_replaceNext->installEventFilter(this);
    m_replacePrevious->installEventFilter(this);
    m_replaceAll->installEventFilter(this);
    //    m_replaceLineEdit

    //    QSpacerItem

    m_layout = new QGridLayout(this);    
    m_layout->addWidget(m_findLabel, 0, 0, Qt::AlignLeft);
    m_layout->addWidget(m_findLineEdit, 0, 1);
    m_layout->addWidget(m_findPrevious, 0, 2);
    m_layout->addWidget(m_findNext, 0, 3);
    m_layout->addWidget(m_close, 0, 5, Qt::AlignRight);
    m_layout->addWidget(m_replaceLabel, 1, 0, Qt::AlignLeft);
    m_layout->addWidget(m_replaceLineEdit, 1, 1);
    m_layout->addWidget(m_replacePrevious, 1, 2);
    m_layout->addWidget(m_replaceNext, 1, 3);
    m_layout->addWidget(m_replaceAll, 1, 4);
    m_layout->setContentsMargins(2, 2, 0, 2);
    m_layout->setColumnStretch(1, 1);
    m_layout->setColumnStretch(5, 1);

    //    this->setLayout(m_layout);
    connect(m_close, SIGNAL(clicked()), this, SLOT(hide()));
    connect(m_findLineEdit, SIGNAL(textEdited(QString)), m_textEdit, SLOT(findExpretion(QString)));
    connect(this, SIGNAL(findNextSignal()), this, SLOT(findNext()));
    connect(this, SIGNAL(findPreviousSignal()), this, SLOT(findPrevious()));
    connect(m_findPrevious, SIGNAL(clicked()), this, SLOT(findPrevious()));
    connect(m_findNext, SIGNAL(clicked()), this, SLOT(findNext()));
    connect(m_replacePrevious, SIGNAL(clicked()), this, SLOT(replacePrevious()));
    connect(m_replaceNext, SIGNAL(clicked()), this, SLOT(replaceNext()));
    connect(m_replaceAll, SIGNAL(clicked()), this, SLOT(replaceAll()));
}

FindWidget::~FindWidget()
{
}

void FindWidget::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event);
    m_textEdit->clearFind();
    m_textEdit->setFocus(Qt::TabFocusReason);
    event->accept();
}

void FindWidget::showEvent(QShowEvent *event)
{
    if (m_textEdit->textCursor().hasSelection()) {
        m_findLineEdit->setText(m_textEdit->textCursor().selectedText());       
    } else {
        m_textEdit->clearFind();
    }

    event->accept();
}

bool FindWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress && obj == m_findLineEdit) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if ((keyEvent->key() == Qt::Key_Enter &&
             keyEvent->modifiers() == Qt::SHIFT) ||
            (keyEvent->key() == Qt::Key_Return &&
             keyEvent->modifiers() == Qt::SHIFT)) {
            m_textEdit->findExpretion(m_findLineEdit->text(), QTextDocument::FindBackward);
            return true;
        } else if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) {
            m_textEdit->findExpretion(m_findLineEdit->text(), m_findFlags);
            return true;
        }
        return QObject::eventFilter(obj, event);
    } else if(event->type() == QEvent::KeyPress && obj == m_replaceLineEdit) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if ((keyEvent->key() == Qt::Key_Enter &&
             keyEvent->modifiers() == Qt::SHIFT) ||
            (keyEvent->key() == Qt::Key_Return && keyEvent->modifiers() == Qt::SHIFT))
        {
            if (!m_replaceLineEdit->text().isEmpty()){
                if (m_textEdit->textCursor().hasSelection() &&
                    m_textEdit->textCursor().selectedText() == m_findLineEdit->text())
                {
                    m_textEdit->replaceExpretion(m_replaceLineEdit->text());
                    m_textEdit->findExpretion(m_findLineEdit->text(), QTextDocument::FindBackward);
                } else{
                    m_textEdit->findExpretion(m_findLineEdit->text(), QTextDocument::FindBackward);
                    m_replaceNext->click();
                }
                return true;
            }
        } else if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) {
            if (!m_replaceLineEdit->text().isEmpty() && !m_findLineEdit->text().isEmpty()){
                if (m_textEdit->textCursor().hasSelection() &&
                    m_textEdit->textCursor().selectedText() == m_findLineEdit->text())
                {
                    m_textEdit->replaceExpretion(m_replaceLineEdit->text());
                    m_textEdit->findExpretion(m_findLineEdit->text());
                } else{
                    m_textEdit->findExpretion(m_findLineEdit->text());
                    m_replaceNext->click();
                }
                return true;
            }
        }
        return QObject::eventFilter(obj, event);
    } else if(event->type() == QEvent::KeyPress && obj == m_replaceAll){
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return){
            replaceAll();
            return true;
        }
        return QObject::eventFilter(obj, event);
    } else {
        return QObject::eventFilter(obj, event);
    }
}

void FindWidget::replacePrevious()
{
    if (!m_replaceLineEdit->text().isEmpty() && !m_findLineEdit->text().isEmpty()){
        if (m_textEdit->textCursor().hasSelection() &&
            m_textEdit->textCursor().selectedText() == m_findLineEdit->text())
        {
            m_textEdit->replaceExpretion(m_replaceLineEdit->text());
            m_textEdit->findExpretion(m_findLineEdit->text(), QTextDocument::FindBackward);
        } else{
            m_textEdit->findExpretion(m_findLineEdit->text(), QTextDocument::FindBackward);
            m_replaceNext->click();
        }
    }
}

void FindWidget::replaceNext()
{
    if (!m_replaceLineEdit->text().isEmpty() && !m_findLineEdit->text().isEmpty()){
        if (m_textEdit->textCursor().hasSelection() &&
            m_textEdit->textCursor().selectedText() == m_findLineEdit->text())
        {
            m_textEdit->replaceExpretion(m_replaceLineEdit->text());
            m_textEdit->findExpretion(m_findLineEdit->text());
        } else{
            m_textEdit->findExpretion(m_findLineEdit->text());
            m_replaceNext->click();
        }
    }
}

void FindWidget::replaceAll()
{
    QString oldText = m_findLineEdit->text();
    QString newText = m_replaceLineEdit->text();

    QTextCursor cursor = m_textEdit->textCursor();
    QTextCursor finalCursor = cursor;
    cursor.setPosition(0, QTextCursor::MoveAnchor);
    m_textEdit->setTextCursor(cursor);
    cursor.beginEditBlock();

    // Replace all we can
    while (true){
        bool r = m_textEdit->find(oldText); //,flags
        if (r){
            QTextCursor qc = m_textEdit->textCursor();
            if (qc.hasSelection())
                qc.insertText(newText);
        } else {
            break;
        }
    }

    cursor.endEditBlock();
    m_textEdit->setTextCursor(finalCursor);
}

void FindWidget::findPrevious()
{
    m_textEdit->findExpretion(m_findLineEdit->text(), QTextDocument::FindBackward);
}

void FindWidget::findNext()
{
    m_textEdit->findExpretion(m_findLineEdit->text(), m_findFlags);
}

