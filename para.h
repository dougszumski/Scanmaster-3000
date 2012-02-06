/* Scanmaster-3000 parameter window
/
/  Copyright 2011, 2012 Doug Szumski <d.s.szumski@gmail.com>
/
/  This file is part of Scanmaster-3000.
/
/    Scanmaster-3000 is free software: you can redistribute it and/or modify
/    it under the terms of the GNU General Public License as published by
/    the Free Software Foundation, either version 3 of the License, or
/    (at your option) any later version.
/
/    Scanmaster-3000 is distributed in the hope that it will be useful,
/    but WITHOUT ANY WARRANTY; without even the implied warranty of
/    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/    GNU General Public License for more details.
/
/    You should have received a copy of the GNU General Public License
/    along with Scanmaster-3000.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PARA_H
#define PARA_H
#include <QDialog>
#include "hist_plot.h"
#include "data_plot.h"

class para : public QDialog
{
    Q_OBJECT
 public:
     para(QWidget * parent = 0, Qt::WindowFlags f = 0);
     ~para();
 signals:
     void intervalCount(int);
     void xMinPosition(double);
     void xMaxPosition(double);
     void scaleFactor(double);
     void HSRes(double);
     void LSRes(double);
     void TVSLevel(double);
     void Th1(double);
     void Th2(double);
     void Th3(double);

};

#endif // PARA_H
