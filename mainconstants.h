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

#ifndef MAINCONSTANTS_H
#define MAINCONSTANTS_H

#include <QtCore/QtGlobal>

//namespace Core {
//namespace Constants {

#define TE_VERSION_MAJOR 0
#define TE_VERSION_MINOR 1
#define TE_VERSION_RELEASE 0

#define STRINGIFY_INTERNAL(x) #x
#define STRINGIFY(x) STRINGIFY_INTERNAL(x)

#define TE_VERSION STRINGIFY(TE_VERSION_MAJOR) \
    "." STRINGIFY(TE_VERSION_MINOR) \
    "." STRINGIFY(TE_VERSION_RELEASE)

const char * const TE_VERSION_LONG      = TE_VERSION;
const char * const TE_AUTHOR            = "Hamed Nemati";
const char * const TE_YEAR              = "2010";

#ifdef TE_REVISION
const char * const TE_REVISION_STR      = STRINGIFY(TE_REVISION);
#else
const char * const TE_REVISION_STR      = "";
#endif

//#undef TE_VERSION
//#undef STRINGIFY
//#undef STRINGIFY_INTERNAL

//} // namespace Constants
//} // namespace Core

#endif // MAINCONSTANTS_H
