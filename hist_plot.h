/* Plot histogram of DAQ data
/
/  Based on work from the Qwt Widget Library
/  Copyright 2011, 2012 Doug Szumski <d.s.szumski@gmail.com>
/  Copyright 1997       Josef Wilgen
/  Copyright 2002       Uwe Rathmann
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


#ifndef _HIST_PLOT_H
#define _HIST_PLOT_H

#include <QWidget>
#include <qwt_plot.h>
#include <qwt_interval_data.h>
#include "histogram_item.h"
#include <qwt_plot_grid.h>

//Default settings
#define DEF_INTERVALS 250
#define DEF_XMIN -1
#define DEF_XMAX 5

class HistPlot : public QwtPlot
{
    Q_OBJECT
public:
    HistPlot(QWidget* = NULL);
    int numIntervals;
public slots:
    void updatePlot(QwtArray<double> values);
    void clearPlot(void);
    void setIntervals(int);
    void setxStart(double);
    void setxStop(double);
private:
    QwtArray<QwtDoubleInterval> intervals;
    QwtArray<double> histdat;
    HistogramItem *histogram;
    QwtPlotGrid *grid;
    int numValues;
    double binwidth;
    //x-axis range
    double start;
    double finish;
};

#endif // HISTPLOT_H
