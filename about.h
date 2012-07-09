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

#ifndef ABOUT_H
#define ABOUT_H

#include "ui_about.h"

class About : public QDialog
{
    Q_OBJECT

public:
    explicit About(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::About ui;

};

#endif // ABOUT_H
