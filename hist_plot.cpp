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
    numIntervals = 10;
    start=-3.0;
    finish=10.0;
    setIntervals(numIntervals);

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

void HistPlot::setxStart(int xStart)
{
    //Update the histogram x-minimum
    start = xStart;
    setAxisScale(QwtPlot::xBottom, start, finish);
    //Recalculate the histogram intervals
    setIntervals(numIntervals);
    show();
}

void HistPlot::setxStop(int xStop)
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
