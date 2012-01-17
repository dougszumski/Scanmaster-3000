#ifndef _HIST_PLOT_H
#define _HIST_PLOT_H

#include <QWidget>
#include <qwt_plot.h>
#include <qwt_interval_data.h>
#include "histogram_item.h"
#include <qwt_plot_grid.h>


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
    void setxStart(int);
    void setxStop(int);
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
