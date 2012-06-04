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

#include "mainconstants.h"
#include "soulnote.h"
#include <QtCore/QDir>
#include <QtCore/QTextStream>
#include <QtCore/QFileInfo>
#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include <QtCore/QLibraryInfo>
#include <QtCore/QTranslator>
#include <QtCore/QVariant>

#include <QtGui/QMessageBox>
#include <QtGui/QApplication>
#include <QtGui/QMainWindow>

#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>

enum { OptionIndent = 4, DescriptionIndent = 24 };

static const char *appNameC = "SoulNote";
//static const char *corePluginNameC = "Core";
static const char *fixedOptionsC =
        " [OPTION]... [FILE]...\n"
        "Options:\n"
        "   -help               Display this help\n"
        "   -version            Display program version\n";

static const char *HELP_OPTION1 = "-h";
static const char *HELP_OPTION2 = "-help";
static const char *HELP_OPTION3 = "/h";
static const char *HELP_OPTION4 = "--help";
static const char *VERSION_OPTION1 = "-version";
static const char *VERSION_OPTION2 = "-v";

#ifdef Q_OS_WIN
static inline void toHtml(QString &t)
{
    t.replace(QLatin1Char('&'), QLatin1String("&amp;"));
    t.replace(QLatin1Char('<'), QLatin1String("&lt;"));
    t.replace(QLatin1Char('>'), QLatin1String("&gt;"));
    t.insert(0, QLatin1String("<html><pre>"));
    t.append(QLatin1String("</pre></html>"));
}

static void displayHelpText(QString t)
{
    toHtml(t);
    QMessageBox::information(0, QLatin1String(appNameC), t);
}

static void displayError(const QString &t)
{
    QMessageBox::critical(0, QLatin1String(appNameC), t);
}

#else

static void displayHelpText(const QString &t)
{
    qWarning("%s", qPrintable(t));
}

/*static void displayError(const QString &t)
{
    qCritical("%s", qPrintable(t));
}*/

#endif


/*static QS Version()
{
    return TE_VERSION;
}*/

static void printVersion()
{
    QString version;
    QTextStream str(&version);
    str << '\n' << appNameC << ' ' << TE_VERSION  << " based on Qt " << qVersion() << "\n\n";
    displayHelpText(version);
}

static void printHelp(const QString &a0)
{
    QString help;
    QTextStream str(&help);
    str << "\nUsage: " << a0  << fixedOptionsC;
    displayHelpText(help);
}

static inline QString msgCoreLoadFailure(const QString &why)
{
    return QCoreApplication::translate("Application", "Failed to load: %1").arg(why);
}

static inline QString msgSendArgumentFailed()
{
    return QCoreApplication::translate("Application", "Unable to send command line arguments to the already running instance. It appears to be not responding!");
}

static inline QString prepareRemoteArgument(const QString &a)
{
    QFileInfo fi(a);
    if (!fi.exists())
        return a;
    if (fi.isRelative())
        return fi.absoluteFilePath();
    return a;
}

int main(int argc, char **argv)
{

    #ifdef Q_WS_X11
        bool useGUI = getenv("DISPLAY") != 0;
    #else
        bool useGUI = true;
    #endif

    QApplication app(argc, argv, useGUI); //QLatin1String

    if (!useGUI)
    {
        printHelp(QFileInfo(app.applicationFilePath()).baseName());
        qWarning("This is not a command like editor. Use Nano or s.th else instead.");
        return 0;
    }


    QTranslator translator;
    //    QTranslator qtTranslator;
    QString locale = QLocale::system().name();
    const QString &creatorTrPath = QCoreApplication::applicationDirPath() + QLatin1String("/../translations");
    if (translator.load(QLatin1String("soulnote_") + locale, creatorTrPath)) {
        app.installTranslator(&translator);
        app.setProperty("sl_locale", locale);
        if (locale == "fa_IR" || locale == "ar")
            qApp->setLayoutDirection(Qt::RightToLeft);

        /*
        const QString &qtTrPath = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
        const QString &qtTrFile = QLatin1String("soulnote_") + locale;
        // Binary installer puts Qt tr files into creatorTrPath
        if (qtTranslator.load(qtTrFile, qtTrPath) || qtTranslator.load(qtTrFile, creatorTrPath)) {
            app.installTranslator(&translator);
            app.installTranslator(&qtTranslator);
            app.setProperty("qtc_locale", locale);
        } else {
            translator.load(QString()); // unload()
        }*/
    }

    const QStringList arguments = app.arguments();
    if (arguments.size() > 1) {
        if (app.arguments().contains(QLatin1String(HELP_OPTION1))
            || app.arguments().contains(QLatin1String(HELP_OPTION2))
            || app.arguments().contains(QLatin1String(HELP_OPTION3))
            || app.arguments().contains(QLatin1String(HELP_OPTION4))) {
            printHelp(QFileInfo(app.applicationFilePath()).baseName());
            return 0;
        }

        if (app.arguments().contains(QLatin1String(VERSION_OPTION1))
            || app.arguments().contains(QLatin1String(VERSION_OPTION2))) {
            printVersion();
            return 0;
        }
    }

    SoulNote *window = new SoulNote();
    window->show();

    try {
        return app.exec();
    } catch (const std::bad_alloc &) {
        //! \todo Save the session and close all config files.
        qCritical("Oops! An Unhandled Error Occurred, The Program Must Be Closed :(");
        return 1;
    }
}
