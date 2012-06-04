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

#ifndef FINDWIDGET_H
#define FINDWIDGET_H

#include <QWidget>
#include <QTextDocument>

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QPushButton;
class QGridLayout;
class TextEdit;
QT_END_NAMESPACE

class FindWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FindWidget(TextEdit *parent = 0);
    virtual ~FindWidget();    

    QLineEdit *m_findLineEdit;

private:
    bool eventFilter(QObject *, QEvent *);

    TextEdit *m_textEdit;

    QLabel *m_findLabel;
    QLabel *m_replaceLabel;
    QLineEdit *m_replaceLineEdit;
    QPushButton *m_findPrevious;
    QPushButton *m_findNext;
    QPushButton *m_replacePrevious;
    QPushButton *m_replaceNext;
    QPushButton *m_replaceAll;
    QPushButton *m_close;
    QGridLayout *m_layout;
    QTextDocument::FindFlags m_findFlags;

protected:
    void hideEvent(QHideEvent *);
    void showEvent(QShowEvent *);

signals:
    void find(QString);
    void findNextSignal();
    void findPreviousSignal();

private slots:
    void replacePrevious();
    void replaceNext();
    void replaceAll();

public slots:
    void findPrevious();
    void findNext();
//    void hideWidget();

};

#endif // FINDWIDGET_H
