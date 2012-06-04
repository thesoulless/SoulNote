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

#include <QtCore/QDebug>
#include <QtCore/QtConcurrentRun>
#include <QtGui/QPainter>
#include <QtGui/QPrinter>
#include <QtGui/QFileDialog>
#include <QtGui/QPrintDialog>
#include <QtGui/QPrintPreviewDialog>
#include <QtCore/QSettings>
#include <QtGui/QLineEdit>

#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QPushButton>
#include <QtGui/QDialogButtonBox>

#include <QtGui/QGridLayout>
#include <QtGui/QMessageBox>
#include <QTextCodec>

#include <QtGui/QGtkStyle>
#include <QtGui/QPalette>
#include <QPageSetupDialog>
#include <QDesktopServices>

#include "soulnote.h"
#include "about.h"

SoulNote::SoulNote(QWidget *parent) :
        QMainWindow(parent){

    ui.setupUi(this);

    m_printer = new QPrinter();
    textEditor = new TextEdit(this);
    m_centralWidget = new QWidget(this);
    m_gridLayout = new QGridLayout(m_centralWidget);
    m_gridLayout->setSpacing(0);
    m_gridLayout->setMargin(0);

    setCentralWidget(m_centralWidget);
    m_centralWidget->setLayout(m_gridLayout);
    m_gridLayout->addWidget(textEditor, 2, 0);

    setWindowTitle("Unsaved Document - SoulNote");
    textEditor->setDocumentTitle("Unsaved Document");

    findWidget = new FindWidget(textEditor);
    m_gridLayout->addWidget(findWidget, 3, 0);
    findWidget->setVisible(false);

    m_goToLineEdit = new QLineEdit(this);
    m_goToLineEdit->setMaxLength(7);
    m_goToLineEdit->setMinimumWidth(70);
    m_goToLineEdit->setMaximumWidth(200);
    m_goToLineEdit->setMinimumHeight(26);
    m_goToLineEdit->setMaximumHeight(26);
    m_goToLineEdit->setContentsMargins(0, 0, 0, 0);
    m_goToLineEdit->setPlaceholderText("Go to line...");    

    QRegExp rx("^[1-9][0-9]{1,6}$");
    QValidator *goToValidator = new QRegExpValidator(rx, this);
    m_goToLineEdit->setValidator(goToValidator);
    m_goToLineEdit->installEventFilter(this);
    ui.statusbar->setContentsMargins(0, 0, 0, 0);
    ui.statusbar->layout()->setContentsMargins(0, 0, 0, 0);

    statusBar()->addWidget(m_goToLineEdit);

    m_fileCodec = QTextCodec::codecForName("UTF-8");
    m_wasHidden = false;
    m_about = 0;

    ui.action_Toolbar->setChecked(false);
    ui.toolBar->hide();
    ui.action_Statusbar->setChecked(true);
    ui.action_ShowLineNumbers->setChecked(true);

    readSettings();
    setCurrentFile("");


    qApp->setOrganizationDomain("http://soulless.ir/");
    QCoreApplication::setApplicationName(QLatin1String("SoulNote"));
    QCoreApplication::setApplicationVersion(QLatin1String("0.0.1")); //Core::Constants::TE_VERSION_LONG
    QCoreApplication::setOrganizationName(QLatin1String("Soulless"));
    QSettings::setDefaultFormat(QSettings::IniFormat);


    createActions();
    createMenus();    

    //QStyle *style = this->style();
    /*! \todo Add fallback icon! */
    ui.action_New->setIcon(QIcon::fromTheme("document-new"));
    ui.action_Open->setIcon(QIcon::fromTheme("document-open"));
    ui.action_Save->setIcon(QIcon::fromTheme("document-save"));
    ui.action_SaveAs->setIcon(QIcon::fromTheme("document-save-as"));
    ui.action_Print->setIcon(QIcon::fromTheme("document-print"));
    ui.action_PrintPreview->setIcon(QIcon::fromTheme("document-print-preview"));
    exitAct->setIcon(QIcon::fromTheme("application-exit"));
    ui.action_Undo->setIcon(QIcon::fromTheme("edit-undo"));
    ui.action_Redo->setIcon(QIcon::fromTheme("edit-redo"));

    ui.action_Cut->setIcon(QIcon::fromTheme("edit-cut"));
    ui.action_Copy->setIcon(QIcon::fromTheme("edit-copy"));
    ui.action_Paste->setIcon(QIcon::fromTheme("edit-paste"));
    ui.action_Delete->setIcon(QIcon::fromTheme("edit-delete"));
    ui.action_SelectAll->setIcon(QIcon::fromTheme("edit-select-all"));
    ui.action_FindReplace->setIcon(QIcon::fromTheme("edit-find-replace"));
    ui.action_GoToLine->setIcon(QIcon::fromTheme("go-jump"));
    ui.action_Preferences->setIcon(QIcon::fromTheme("preferences-system")); //emblem-system
    ui.action_Fullscreen->setIcon(QIcon::fromTheme("view-fullscreen"));
    ui.action_About->setIcon(QIcon::fromTheme("help-about"));


    connect(textEditor->document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()) );
    connect(this, SIGNAL(setEditorText(QString)), textEditor, SLOT(setPlainText(QString)));
    connect(this, SIGNAL(insertEditorText(QString)), textEditor, SLOT(insertPlainText(QString)));
    connect(textEditor->document(), SIGNAL(redoAvailable(bool)), ui.action_Redo, SLOT(setEnabled(bool)));
    connect(textEditor->document(), SIGNAL(undoAvailable(bool)), ui.action_Undo, SLOT(setEnabled(bool)));
    connect(textEditor, SIGNAL(copyAvailable(bool)), ui.action_Copy, SLOT(setEnabled(bool)));
    connect(textEditor, SIGNAL(copyAvailable(bool)), ui.action_Cut, SLOT(setEnabled(bool)));
}

void SoulNote::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui.retranslateUi(this);
        break;
    default:
        break;
    }
}

bool SoulNote::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress && obj == m_goToLineEdit) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) {
            if (m_wasHidden){
                ui.action_Statusbar->setChecked(false);
                on_action_Statusbar_triggered();
                m_wasHidden = false;
            }
            if (!m_goToLineEdit->text().isEmpty()) {
                textEditor->gotoLine(m_goToLineEdit->text().toInt(), textEditor->textCursor().positionInBlock());
                textEditor->setFocus(Qt::OtherFocusReason);                
            }
            return true;
        }
        return QObject::eventFilter(obj, event);
    } else {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}

QString SoulNote::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void SoulNote::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    /*! \todo Activate it! !*/
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void SoulNote::exit_triggered()
{
    this->close();
}




/****** Initialization *****/
void SoulNote::createActions()
{
    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
    }

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(exit_triggered()));
}

void SoulNote::createMenus()
{
    separatorAct = ui.menu_SoulNote->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
        ui.menu_SoulNote->addAction(recentFileActs[i]);
    ui.menu_SoulNote->addSeparator();
    updateRecentFileActions();

    ui.menu_SoulNote->addAction(exitAct);
}

void SoulNote::updateRecentFileActions()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Soulless", "SoulNote");
    settings.beginGroup("Window");

    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles - 1; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);

    separatorAct->setVisible(numRecentFiles > 0);
    settings.endGroup();
}

void SoulNote::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        openDocument(action->data().toString());
}
/****** End Of Initialization *****/



/****** File Menu Actions ******/
void SoulNote::on_action_New_triggered()
{
    SoulNote *soulnote = new SoulNote();
    soulnote->show();
}

void SoulNote::on_action_Open_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select files to open..."), "");

    openDocument(fileName);
}

void SoulNote::on_action_Save_triggered()
{
    save();
}

void SoulNote::on_action_SaveAs_triggered()
{
    saveDocument();
}

void SoulNote::on_action_Print_triggered()
{
    /*! \todo add document title, date and time & .... !*/
#ifndef QT_NO_PRINTER
    QPrintDialog *dialog = new QPrintDialog(m_printer, this);
    QPrintDialog d(m_printer, this);
    dialog->setWindowTitle(tr("Print Document"));
    d.setWindowTitle(tr("Print Document"));
    if (d.exec() != QDialog::Accepted)
        return;
    textEditor->print(m_printer);
#endif
}

void SoulNote::on_action_PageSetup_triggered()
{
    /*! \todo remove page setup because it's just plain text! !*/
}

void SoulNote::on_action_PrintPreview_triggered()
{
    QPrintPreviewDialog *preview = new QPrintPreviewDialog(m_printer, this);
    connect(preview, SIGNAL(paintRequested(QPrinter*)), this, SLOT(preview(QPrinter*)));
    preview->exec();
}

void SoulNote::preview(QPrinter*)
{
    textEditor->print(m_printer);

    /*! \todo Add document title & datetime & ... to printing optopns !*/
}
/****** End Of File Menu Actions ******/

void SoulNote::on_action_Undo_triggered()
{
    if (textEditor->isUndoRedoEnabled())
        textEditor->undo();
}

void SoulNote::on_action_Redo_triggered()
{
    if (textEditor->isUndoRedoEnabled())
        textEditor->redo();
}

void SoulNote::on_action_Cut_triggered()
{
    if (textEditor->textCursor().hasSelection())
        textEditor->cut();
}

void SoulNote::on_action_Copy_triggered()
{
    if (textEditor->textCursor().hasSelection())
        textEditor->copy();
}

void SoulNote::on_action_Paste_triggered()
{
    if (textEditor->canPaste())
        textEditor->paste();
}

void SoulNote::on_action_Delete_triggered()
{
    if (textEditor->textCursor().hasSelection())
        textEditor->textCursor().removeSelectedText();
}

void SoulNote::on_action_SelectAll_triggered()
{
    textEditor->selectAll();
}

void SoulNote::on_action_FindReplace_triggered()
{
    if (findWidget->isVisible()) {
        findWidget->m_findLineEdit->setFocus(Qt::OtherFocusReason);
        findWidget->m_findLineEdit->selectAll();
    } else {
        findWidget->show();
        findWidget->m_findLineEdit->setFocus(Qt::OtherFocusReason);
        findWidget->m_findLineEdit->selectAll();
    }
}

void SoulNote::on_action_FindNext_triggered()
{
    findWidget->findNext();
}

void SoulNote::on_action_FindPrevious_triggered()
{
    findWidget->findPrevious();
}

void SoulNote::on_action_GoToLine_triggered()
{
    if (ui.statusbar->isHidden()){
        ui.action_Statusbar->setChecked(true);
        on_action_Statusbar_triggered();
        m_wasHidden = true;
    }
    m_goToLineEdit->setFocus(Qt::OtherFocusReason);
    m_goToLineEdit->selectAll();
}

void SoulNote::on_action_Preferences_triggered()
{
    /*!  */
}

void SoulNote::on_action_Toolbar_triggered()
{
    if (!ui.action_Toolbar->isChecked())
        ui.toolBar->hide();
    else
        ui.toolBar->setVisible(true);
}

void SoulNote::on_action_Statusbar_triggered()
{
    if (!ui.action_Statusbar->isChecked())
        ui.statusbar->hide();
    else
        ui.statusbar->setVisible(true);
}

void SoulNote::on_action_Fullscreen_triggered(bool checked)
{
    setFullScreen(checked);
}

void SoulNote::on_action_ShowLineNumbers_triggered()
{
    if (ui.action_ShowLineNumbers->isChecked())
        textEditor->setLineNumberAreaVisible(true);
    else
        textEditor->setLineNumberAreaVisible(false);
}

void SoulNote::on_action_DocumentStatics_triggered()
{

}

void SoulNote::on_action_CommentUncomment_triggered()
{

}

void SoulNote::on_action_About_triggered()
{
    aboutSoulNote();
}
/*****************************
        End of Actions
*****************************/

void SoulNote::aboutSoulNote()
{
    if (!m_about) {
        m_about = new About(this);
        connect(m_about, SIGNAL(finished(int)),
                this, SLOT(destroyAboutDialog()));
    }
    m_about->show();
}

void SoulNote::destroyAboutDialog()
{
    if (m_about) {
        m_about->deleteLater();
        m_about = 0;
    }
}

void SoulNote::documentWasModified()
{    
    setWindowModified(textEditor->document()->isModified());
    if (!windowTitle().contains("[*]") && isWindowModified())
        setWindowTitle("[*]" + windowTitle());
}

QString SoulNote::readFile(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Cannot load the file"),
                             tr("Cannot read the %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        qWarning("%s", qPrintable(tr("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString())));
        ui.statusbar->showMessage(trUtf8("Failed to open file %1").arg(fileName),1000);
        return QString("");
    }

    QTextStream in(&file);
    QString line;

    in.setAutoDetectUnicode(true);    

    while (!in.atEnd()) {
        line += in.readLine();
        if (!in.atEnd())
            line += "\n";
    }

    return line;
}

void SoulNote::openDocument(const QString &fileName)
{
    qApp->setOverrideCursor(Qt::WaitCursor);
    ui.statusbar->showMessage(trUtf8("Loading ..."));
    QFuture<QString> f1 = QtConcurrent::run(this, &SoulNote::readFile, fileName);
    f1.waitForFinished();
    QString result = f1.result();
    qApp->restoreOverrideCursor();
    setCurrentFile(fileName);
    ui.statusbar->showMessage(tr("File %1 loaded").arg(fileName), 3000);
    this->setWindowFilePath(fileName);
    setEditorText(result);
}

bool SoulNote::saveDocument()
{
    QFileDialog *box = new QFileDialog(this, "Save As...", QDir::homePath(), "Text files (*.txt);;All Files (*)");
    box->setViewMode(QFileDialog::Detail);

    box->setOption(QFileDialog::DontUseNativeDialog, false);
    box->setAcceptMode(QFileDialog::AcceptSave);
    QLabel *encodingLabel = new QLabel("Charecter Encoding:" ,box);
    QComboBox *encodingComboBox = new QComboBox(box);
    QMap<QString, QString> encodings;
    encodings["Unicode (UTF-8)"] = "UTF-8";
    encodings["Unicode (UTF-16)"] = "UTF-16";
    encodings["Unicode (UTF-32)"] = "UTF-32";
    //encodings["Shift"] = "Shift-JIS";
    encodingComboBox->addItems(encodings.keys());
    encodingComboBox->setCurrentIndex(3);

    QGridLayout *layout = (QGridLayout*)box->layout();
    Q_ASSERT(layout);
    layout->addWidget(encodingLabel, 4, 0);
    layout->addWidget(encodingComboBox, 4, 1);

    QString fileName;

    if (box->exec() == QDialog::Accepted) {
        QString selectedCodec = encodings.value(encodingComboBox->currentText());
        qDebug() << selectedCodec;
        QTextCodec *codec = QTextCodec::codecForName(selectedCodec.toLatin1());
        Q_ASSERT(codec);
        qDebug() << textEditor->toPlainText();
        qDebug() << "Can Encode? " << codec->canEncode(textEditor->toPlainText());
        if (!codec->canEncode(textEditor->toPlainText())){
            if (QMessageBox::Yes == QMessageBox::question(this, tr("Force to save?!"),
                                                          tr("This file can not be encoded with the "
                                                             "selected encoding. Do you really want "
                                                             "to save with ") + selectedCodec +
                                                          tr(" encoding?!"), QMessageBox::Yes, QMessageBox::No)){
                fileName = box->selectedFiles().value(0);
                m_fileCodec = codec;
            } else {
                saveDocument();
            }
        } else {
            fileName = box->selectedFiles().value(0);
            m_fileCodec = codec;
        }
    }

    this->textEditor->setFocus();
    qDebug() << "Selceted " << fileName << " for save file name.";
    if (fileName.isEmpty())
        return false;

    return saveDocument(fileName);
}

bool SoulNote::saveFile(const QString &fileName)
{
#ifdef Q_WS_WIN
    QFile file(fileName + ".txt");
#else
    QFile file(fileName);
#endif

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        QMessageBox::warning(this, tr("Codecs"),
                             tr("Cannot write file %1:\n%2")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    out.setAutoDetectUnicode(false);
    out.setCodec(m_fileCodec);
    out << textEditor->toPlainText();

    return true;
}

bool SoulNote::save()
{
    if (m_curerntFile.isEmpty()) {
        return saveDocument();
    } else {
        return saveDocument(m_curerntFile);
    }
}

bool SoulNote::saveDocument(const QString &fileName)
{
    qApp->setOverrideCursor(Qt::WaitCursor);
    this->setEnabled(false);
    ui.statusbar->showMessage(trUtf8("Saving ..."));
    QFuture<bool> f1 = QtConcurrent::run(this, &SoulNote::saveFile,
                                         fileName);
    f1.waitForFinished();
    bool result = f1.result();
    setCurrentFile(fileName);
    qApp->restoreOverrideCursor();
    this->setEnabled(true);
    this->update();
    this->textEditor->setFocus();
    if (result){
        ui.statusbar->showMessage(tr("File %1 saved").arg(fileName), 3000);
        return true;
    } else {
        return false;
    }
}

bool SoulNote::maybeSave(){
    if (textEditor->document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("SoulNote"),
                                   tr("The document has been modified.\nDo you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}
void SoulNote::setCurrentFile(const QString &fileName)
{
    m_curerntFile = fileName;
    textEditor->document()->setModified(false);
    setWindowModified(false);
    QString shownName;

    if (fileName.isEmpty())
        shownName = "unsaved document";
    else {
        this->setWindowFilePath(fileName);
        shownName = strippedName(fileName);
    }

    setWindowTitle(tr("[*]%1 - %2").arg(shownName).arg(tr("SoulNote")));

    if ( !fileName.isEmpty() )
    {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Soulless", "SoulNote");
        settings.beginGroup("Window");
        QStringList files = settings.value("recentFileList").toStringList();
        files.removeAll(fileName);
        files.prepend(fileName);
        while (files.size() > MaxRecentFiles)
            files.removeLast();

        settings.setValue("recentFileList", files);
        settings.endGroup();

        foreach (QWidget *widget, QApplication::topLevelWidgets()) {
            SoulNote *mainWin = qobject_cast<SoulNote *>(widget);
            if ( mainWin )
                mainWin->updateRecentFileActions();
        }
    }
}

void SoulNote::setFullScreen(bool on)
{
    if (bool(windowState() & Qt::WindowFullScreen) == on)
        return;

    if (on) {
        setWindowState(windowState() | Qt::WindowFullScreen);
    } else {
        setWindowState(windowState() & ~Qt::WindowFullScreen);
    }
}

//static const char *settingsGroup = "MainWindow";
static const char *geometryKey = "Geometry";
//static const char *colorKey = "Color";
static const char *maxKey = "Maximized";
static const char *fullScreenKey = "FullScreen";
static const char *WordWrapping = "WordWrap";
static const char *LineWrapping  = "Wrapping";

/*!
    Write settings of the program in an INI file.
    \fn SN::writeSettings()    
  */
void SoulNote::writeSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Soulless", "SoulNote");

    settings.beginGroup("MainWindow");
    //settings.setValue("Font", textEditor->currentFont());
    settings.setValue(QLatin1String(LineWrapping), textEditor->lineWrapMode());
    settings.setValue(QLatin1String(WordWrapping), textEditor->wordWrapMode());
    settings.setValue("FilePath",this->windowFilePath());
    //settings.setValue("WindowState", Qt::WindowState);



    if (windowState() & (Qt::WindowMaximized | Qt::WindowFullScreen)) {
        settings.setValue(QLatin1String(maxKey), (bool) (windowState() & Qt::WindowMaximized));
        settings.setValue(QLatin1String(fullScreenKey), (bool) (windowState() & Qt::WindowFullScreen));
    } else {
        settings.setValue(QLatin1String(maxKey), false);
        settings.setValue(QLatin1String(fullScreenKey), false);
        settings.setValue(QLatin1String(geometryKey), geometry());
    }
    //settings.setValue("Colors", this->colorScheme);
    settings.endGroup();
}

void SoulNote::readSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Soulless", "SoulNote");
    settings.beginGroup("MainWindow");

    const QVariant geom = settings.value(QLatin1String(geometryKey));

    if (geom.isValid()) {
        setGeometry(geom.toRect());
    } else {
        resize(1024, 768);
    }

    if (settings.value(QLatin1String(maxKey), false).toBool())
        setWindowState(Qt::WindowMaximized);

    setFullScreen(settings.value(QLatin1String(fullScreenKey), false).toBool());
    //textEditor->setFont(settings.value("Font", QFont("Monospace", 10)).value<QFont>());
    textEditor->setLineWrapMode(QPlainTextEdit::LineWrapMode(settings.value(QLatin1String(LineWrapping), 1).toInt()));
    textEditor->setWordWrapMode(QTextOption::WrapMode(settings.value(QLatin1String(WordWrapping), 4).toInt()));
    settings.endGroup();
}
