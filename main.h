/* Scanmaster-3000
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

#include <QMainWindow>
#include "para.h"

#ifndef MAIN_H
#define MAIN_H

class MainWindow: public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow();
    private slots:
        void VersionMessage(void);
        void save(void);
        void showpars(void);
    private:
        QMenu *fileMenu;
        QMenu *DAQMenu;
        QMenu *plotMenu;
        QMenu *aboutMenu;
        QAction *exitAct;
        QAction *setFileAct;
        QAction *DAQStartAct;
        QAction *DAQStopAct;
        QAction *DAQAqAct;
        QAction *clearPlotAct;
        QAction *aboutAct;
        QAction *setParametersAct;
        para *setHistPara;
    signals:
        void newFilename(QString);

};

#endif // MAIN_H
