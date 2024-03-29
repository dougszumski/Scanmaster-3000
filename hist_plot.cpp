/* Plot histogram - designed for updating with every sample of the DAQ
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

#include "hist_plot.h"
#include <stdlib.h>
#include <qapplication.h>
#include <qpen.h>
#include <qwt_plot.h>
#include <qwt_plot_marker.h>
#include <qwt_interval_data.h>
#include "histogram_item.h"
#include <QDebug>


HistPlot::HistPlot(QWidget *parent):

        QwtPlot(parent)
{
   
    //Set defaults
    start=DEF_XMIN;
    finish=DEF_XMAX;
    setIntervals(DEF_INTERVALS);

    //Setup the plot
    setCanvasBackground(QColor(Qt::black));
    setTitle("Current Histogram");
    grid = new QwtPlotGrid;
    grid->enableXMin(true);
    grid->enableYMin(true);
    grid->setMajPen(QPen(Qt::black, 0, Qt::DotLine));
    grid->setMinPen(QPen(Qt::gray, 0 , Qt::DotLine));
    grid->attach(this);
    setAxisScale(QwtPlot::xBottom, start, finish);
    setAxisTitle(QwtPlot::xBottom, "Current (nA)");
    setAxisTitle(QwtPlot::yLeft, "Counts");
    resize(800,200);
    show();

    //Create the histogram
    histogram = new HistogramItem();
    histogram->setColor(Qt::gray);
    histogram->attach(this);

}

void HistPlot::setxStart(double xStart)
{
    //Update the histogram x-minimum
    start = xStart;
    setAxisScale(QwtPlot::xBottom, start, finish);
    //Recalculate the histogram intervals
    setIntervals(numIntervals);
    show();
}

void HistPlot::setxStop(double xStop)
{
    //Update the histogram x-minimum
    finish = xStop;
    setAxisScale(QwtPlot::xBottom, start, finish);
    //Recalculate the histogram intervals
    setIntervals(numIntervals);
    show();
}

void HistPlot::setIntervals(int newNumIntervals)
{
    //Define the histogram bins
    qDebug("Intervals set to: %i", numIntervals);
    numIntervals = newNumIntervals;
    double binpos = start;
    intervals.resize(numIntervals);
    binwidth = (finish-start)/numIntervals;

    for ( int i = 0; i < numIntervals; i++ )
    {
       intervals[i] = QwtDoubleInterval(binpos, binpos + binwidth);
       //qDebug("Bin position: %f", binpos);
       binpos += binwidth;

    }

    //Resize the histogram data array
    histdat.resize(numIntervals);

}

void HistPlot::updatePlot(QwtArray<double> data)

{
    //qDebug("Intervals used here: %i", numIntervals);
    //qDebug() << "*****************Update plot called ******************************";
    //Sort the data, uses merge sort algo. ~nlog(n)
    qSort(data.begin(), data.end());

    //Bin the data into histdat
    numValues = data.size();
    int j = 0;
    double bin_llim = start;

    for ( int i = 0; i < numValues; i++ )
    {
        //Make sure data is in the range of the histogram - useful for trimming bits off
        if ((data[i] >= start) && (data[i] < finish))
        {
            if ( (data[i] < (bin_llim + binwidth) ) && (data[i] >= bin_llim) )
            {
                //If the datapoint is in the bin, then increment it
                histdat[j]++;
            }
            else
            {
                /* Keep moving along the bins until a count is found
                 * or there are no bins left.
                 * Latter check should be guaranteed if start and end defined properly */
                while( (data[i] > (bin_llim + binwidth) ) && (j < numIntervals) )
                {
                   bin_llim += binwidth;
                   j++;
                }
                histdat[j]++;
            }
        }
    }

    //Work out the max bin count for setting the y axis
    double maxVal = histdat[0];
    for (int i=0; i<numIntervals; i++) {
       if (histdat[i] > maxVal) {
            maxVal = histdat[i];
        }
    }

    //Update the histogram and plot
    setAxisScale(QwtPlot::yLeft, 0, maxVal*1.2);;
    histogram->setData(QwtIntervalData(intervals, histdat));
    replot();

}

void HistPlot::clearPlot(void)
{
    setAxisScale(QwtPlot::yLeft, start, finish);
    histdat.fill(0);
    histogram->setData(QwtIntervalData(intervals, histdat));
    replot();
}
