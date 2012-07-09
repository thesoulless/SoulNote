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

#ifndef SOULNOTE_H
#define SOULNOTE_H

//#include <QMainWindow>

#include <QtGui/QLineEdit>
#include "ui_soulnote.h"
#include "textedit.h"
#include "findwidget.h"

/*namespace Ui {
    class SoulNote;
}*/

QT_BEGIN_NAMESPACE
class QPrinter;
class QGridLayout;
class GoToLine;
class QHBoxLayout;
class QTextCodec;
class QSettings;
class About;
class Preferences;
QT_END_NAMESPACE


class SoulNote : public QMainWindow
{
    Q_OBJECT

public:
    explicit SoulNote(QWidget *parent = 0);

    TextEdit *textEditor;
    FindWidget *findWidget;
    void openDocument(const QString &fileName);
    bool saveDocument();
    bool saveDocument(const QString &);

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *);
    void dragEnterEvent(QDragEnterEvent *e);

private:
    bool eventFilter(QObject *, QEvent *);
    Ui::SoulNote ui;
//    GoToLine *goToWidget;
    QLineEdit *m_goToLineEdit;
    QString curentFile;
    QString curentDir; /*! \todo check if needed !*/
    QPrinter *m_printer;
    QWidget *m_centralWidget;
    QGridLayout *m_gridLayout;
    QWidget *m_statusbar;
    QHBoxLayout *m_statusbarLayout;

    void createActions();
    void createMenus();
    void updateRecentFileActions();
    enum { MaxRecentFiles = 5 };
    QAction *separatorAct;
    QAction *exitAct;
    QAction *recentFileActs[MaxRecentFiles];

    QString strippedName(const QString &fullFileName);
    int m_columnNumber;

    QString readFile(const QString &fileName);
    bool saveFile();
    bool saveFile(const QString &);
    bool save();
    QString m_curerntFile;
    QTextCodec *m_fileCodec;

    bool m_wasHidden;
    bool maybeSave();
    About *m_about;
    Preferences *m_preferences;
    void aboutSoulNote();
    void preferencesSoulNote();
//    QSettings *m_settings;

signals:
    void setEditorText(QString);
    void insertEditorText(QString);

private slots:
    void on_action_About_triggered();
    void on_action_CommentUncomment_triggered();
    void on_action_DocumentStatics_triggered();
    void on_action_ShowLineNumbers_triggered();
    void on_action_Fullscreen_triggered(bool checked);
    void on_action_Statusbar_triggered();
    void on_action_Toolbar_triggered();
    void on_action_Preferences_triggered();
    void on_action_GoToLine_triggered();
    void on_action_FindPrevious_triggered();
    void on_action_FindNext_triggered();
    void on_action_FindReplace_triggered();
    void on_action_SelectAll_triggered();
    void on_action_Delete_triggered();
    void on_action_Paste_triggered();
    void on_action_Copy_triggered();
    void on_action_Cut_triggered();
    void on_action_Redo_triggered();
    void on_action_Undo_triggered();
    void on_action_PrintPreview_triggered();
    void on_action_PageSetup_triggered();
    void on_action_Print_triggered();
    void on_action_SaveAs_triggered();
    void on_action_Save_triggered();
    void on_action_Open_triggered();
    void on_action_New_triggered();
    void openRecentFile();
    void preview(QPrinter*);
    void exit_triggered();

    void setFullScreen(bool on);
    void documentWasModified();
    void setCurrentFile(const QString &fileName);
    void writeSettings();
    void readSettings();
    void destroyAboutDialog();
    void destroyPreferencesDialog();

    void dropEvent(QDropEvent *e);
};

class GoToLine : public QWidget
{
public:
    GoToLine(QWidget *soulnote) : QWidget(soulnote) {
//        m_soulnote = soulnote;
        m_goToLine = new QLineEdit(this);
        m_goToLine->setMaxLength(9999999);
        m_goToLine->setMinimumWidth(150);
        m_goToLine->setPlaceholderText("Line Number");
        m_goToLine->setInputMethodHints(Qt::ImhDigitsOnly);
        m_goToLine->installEventFilter(this);
    } //SoulNote

    QLineEdit *m_goToLine;

    /*QSize sizeHint() const {
        return QSize(m_editor->lineNumberAreaWidth(), 0);
    }*/

protected:
    /*void paintEvent(QPaintEvent *event) {
        m_editor->lineNumberAreaPaintEvent(event);
    }*/

private:
    /*bool eventFilter(QObject *obj, QEvent *event)
    {
        if (event->type() == QEvent::KeyPress && obj == m_goToLine) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) {
                if (!m_goToLine->text().isEmpty()) {
                    m_soulnote->textEditor->gotoLine(m_goToLine->text().toInt(), m_soulnote->textEditor->textCursor().columnNumber());
                    m_soulnote->textEditor->setFocus(Qt::OtherFocusReason);
                }
                return true;
            }
            return QObject::eventFilter(obj, event);
        } else {
            // standard event processing
            return QObject::eventFilter(obj, event);
        }
    }*/

//    SoulNote *m_soulnote;
};

#endif // SOULNOTE_H
