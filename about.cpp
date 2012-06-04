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

#include <QDesktopServices>
#include <QUrl>
#include "about.h"

About::About(QWidget *parent) :
    QDialog(parent){
    ui.setupUi(this);
    setContentsMargins(0, 0, 0, 0);
}

void About::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui.retranslateUi(this);
        break;
    default:
        break;
    }
}

void About::on_label_2_linkActivated(QString link)
{
    Q_UNUSED(link);
    //QDesktopServices::openUrl(QUrl(link));
}
