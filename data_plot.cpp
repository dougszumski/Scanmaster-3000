/* Acquire and plot data from a NI DAQ
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

#include <stdlib.h>
#include <qwt_painter.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_widget.h>
#include <qwt_legend.h>
#include <qwt_scale_draw.h>
#include <qwt_math.h>
#include "data_plot.h"
#include "NIDAQmxBase.h"
#include <QtCore/QCoreApplication>
#include <qfile.h>
#include <qtextstream.h>
#include <iostream>
#include <QDebug>
#include <QMessageBox>

//  Initialize main window

DataPlot::DataPlot(QWidget *parent):

        QwtPlot(parent),
        d_interval(0),
        d_timerId(-1)
{
    //Set defaults
    scale = SCALE;
    i_hs_res = HS_RES;
    i_ls_res = LS_RES;
    tvs_switchover = TVS_SWITCH;
    th_1 = TH_1;
    th_2 = TH_2;
    th_3 = TH_3;

    // Disable polygon clipping
    QwtPainter::setDeviceClipping(false);

    // We don't need the cache here
    canvas()->setPaintAttribute(QwtPlotCanvas::PaintCached, false);
    canvas()->setPaintAttribute(QwtPlotCanvas::PaintPacked, false);
    canvas()->setAttribute(Qt::WA_PaintOnScreen, true);
    alignScales();
    
    //  Initialize data
    for (int i = 0; i< PLOT_SIZE; i++)
    {
        d_x[i] =  i;     // time axis
        ai_0[i] = 0;
        ai_1[i] = 0;
        ai_2[i] = 0;
        ai_3[i] = 0;
    }

    // Assign a title
    setTitle("NI-9215A ADC Output");
    insertLegend(new QwtLegend(), QwtPlot::BottomLegend);

    // Set canvas colour
    setCanvasBackground(QColor(Qt::black));

    // Insert new curves
    QwtPlotCurve *cRight = new QwtPlotCurve("Channel 1");
    cRight->attach(this);

    QwtPlotCurve *cRight2 = new QwtPlotCurve("Channel 2");
    cRight2->attach(this);

    QwtPlotCurve *cRight3 = new QwtPlotCurve("Channel 3");
    cRight3->attach(this);

    QwtPlotCurve *cRight4 = new QwtPlotCurve("Channel 4");
    cRight4->attach(this);

    // Set curve styles
    cRight->setPen(QPen(Qt::red));
    cRight2->setPen(QPen(Qt::blue));
    cRight3->setPen(QPen(Qt::green));
    cRight4->setPen(QPen(Qt::white));

    // Attach (don't copy) data. Both curves use the same x array.
    cRight->setRawData(d_x, ai_0, PLOT_SIZE);
    cRight2->setRawData(d_x, ai_1, PLOT_SIZE);
    cRight3->setRawData(d_x, ai_2, PLOT_SIZE);
    cRight4->setRawData(d_x, ai_3, PLOT_SIZE);

    // Axis 
    setAxisTitle(QwtPlot::xBottom, "Time");
    setAxisScale(QwtPlot::xBottom, 0, 1000);

    setAxisTitle(QwtPlot::yLeft, "Voltage (V)");
    setAxisScale(QwtPlot::yLeft, -12, 12);
#ifdef DEBUG
    setTimerInterval(100.0);
#else
    setTimerInterval(10.0);
#endif
}

void DataPlot::initDAQ()
{

    // ADC Stuff
    taskHandle = 0;
    min = -10.0;
    max = 10.0;
    // Data read parameters
    pointsToRead = bufferSize;
    timeout = 10.0;
    samplesPerChan = SAMPLES_PER_CHAN;
    sampleRate = SAMPLE_RATE;
#ifndef DEBUG
    //Setup the task - takes bloody ages
    DAQmxBaseCreateTask("",&taskHandle);
    //Enable all 4 channels
    DAQmxBaseCreateAIVoltageChan(taskHandle,"Dev1/ai0, Dev1/ai1, Dev1/ai2, Dev1/ai3","",DAQmx_Val_Cfg_Default,min,max,DAQmx_Val_Volts,NULL);
    DAQmxBaseCfgSampClkTiming(taskHandle,"OnboardClock",sampleRate,DAQmx_Val_Rising,DAQmx_Val_ContSamps,samplesPerChan);
    DAQmxBaseCfgInputBuffer(taskHandle,40000); //DMA buffer size, overides 'auto' which doesn't work
    DAQmxBaseStartTask(taskHandle);
#endif
    //Data sample which gets sent to the hisogram
    data_sample.resize(samplesPerChan);
    read_data = true;

}


void DataPlot::quitDAQ()
{
    if(taskHandle != 0) {
        DAQmxBaseStopTask (taskHandle);
        DAQmxBaseClearTask (taskHandle);
    }
    read_data = false;
}


//
//  Set a plain canvas frame and align the scales to it
//
void DataPlot::alignScales()
{
    // The code below shows how to align the scales to
    // the canvas frame, but is also a good example demonstrating
    // why the spreaded API needs polishing.

    canvas()->setFrameStyle(QFrame::Box | QFrame::Plain );
    canvas()->setLineWidth(1);

    for ( int i = 0; i < QwtPlot::axisCnt; i++ )
    {
        QwtScaleWidget *scaleWidget = (QwtScaleWidget *)axisWidget(i);
        if ( scaleWidget )
            scaleWidget->setMargin(0);

        QwtScaleDraw *scaleDraw = (QwtScaleDraw *)axisScaleDraw(i);
        if ( scaleDraw )
            scaleDraw->enableComponent(QwtAbstractScaleDraw::Backbone, false);
    }
}


void DataPlot::setTimerInterval(double ms)
{
 
    d_interval = qRound(ms);

    if ( d_timerId >= 0 )
    {
        killTimer(d_timerId);
        d_timerId = -1;
    }
    if (d_interval >= 0 )
        d_timerId = startTimer(d_interval);
}

void DataPlot::logChannel(bool input)
{
    if (localfile.length() > 0) {store_data = input;}
    else{
        //TODO this must really decheck the box!
        QMessageBox msgBox;
        msgBox.setText("No filename specified");
        msgBox.setWindowTitle(tr("Error"));
        msgBox.exec();
    }

}

void DataPlot::inputfile(QString filename){localfile = filename;}

void DataPlot::setHSRes(double newHSRes){i_hs_res = newHSRes;}

void DataPlot::setLSRes(double newLSRes){i_ls_res = newLSRes;}

void DataPlot::setTVSLevel(double newTVSLevel){tvs_switchover = newTVSLevel;}

void DataPlot::setTh1(double newTh1){th_1 = newTh1;}

void DataPlot::setTh2(double newTh2){th_2 = newTh2;}

void DataPlot::setTh3(double newTh3){th_3 = newTh3;}

void DataPlot::setScale(double newScale){
    qDebug("Scale changed to: %f", scale);
    scale = newScale;}

void DataPlot::timerEvent(QTimerEvent *)
//  Read ADC, update the graph and save the data
{
    //Function protype: int32 DAQmxBaseReadAnalogF64 (TaskHandle taskHandle, int32 numSampsPerChan, float64 timeout,
    //bool32 fillMode, float64 readArray[ ], //uInt32 arraySizeInSamps, int32 *sampsPerChanRead, bool32 *reserved);
    if (read_data == true) {

        //Read the data
#ifndef DEBUG
        DAQmxBaseReadAnalogF64(taskHandle,pointsToRead,timeout,DAQmx_Val_GroupByScanNumber,data,bufferSize,&pointsRead,NULL);
#endif
            //Update the graph
            for (int i = (int)samplesPerChan-1; i >= 0 && i < (int)samplesPerChan; i--){
                ai_0[i] = data[(4*i)];
#ifdef DEBUG
                ai_0[i] = ( (rand()%100000)/10000.0 -5.0);
#endif
                ai_1[i] = data[(4*i+1)];
                ai_2[i] = data[(4*i+2)];
                ai_3[i] = data[(4*i+3)];
            }
            replot();

            //Begin by setting the combined channel to the the lowest resolution
            memcpy(ai_combined, ai_0, sizeof(ai_0));

            //Attempt to improve the resolution by stiching the channels together
            for (int i = 0; i < (int)samplesPerChan; i++)
            {
                if (fabs(ai_combined[i]) < th_1 ){
                    //Output below threshold so try and replace it with a higher sensitivity measurement
                    if (fabs(ai_1[i]) > th_2){
                        //LSx10 channel is still in operation so use the measurment from there
                        ai_combined[i] = ai_1[i] / (i_ls_res * 10) * scale;}
                    else if ((fabs(ai_2[i]) > th_3) && (fabs(ai_2[i]) < tvs_switchover)){
                        //Limiter is off, HSx1 channel is in operation and the TVS diode is off
                        ai_combined[i] = ai_2[i] / i_hs_res * scale; }
                    else {
                        //If HSx1 is below threshold then use the HSx10 channel regardless
                        ai_combined[i] = ai_3[i] / (i_hs_res * 10) * scale;}
                 }
                 else {
                    //LSx1 channel is fine so convert to current
                    ai_combined[i] = ai_combined[i] / (i_ls_res) * scale;
                 }
            }

            //Copy data from array to QwtArray - is there someway of casting it or using memcpy?
            for (uint i = 0; i < samplesPerChan; i++){
                data_sample[i] = log10(fabs(ai_combined[i]));

            }
            
            //Send data to histogram
            emit dataSample(data_sample);

            if (store_data == true)
            {
                QFile outputfile(localfile);
                if (outputfile.open(QFile::WriteOnly | QIODevice::Append))
                {
                    QTextStream out(&outputfile);
                    for (uint j = 0; j < samplesPerChan; j++)
                    {
                        QString newLine = QString( " %0 \t %1 \t %2 \t %3 \n" )
                                          .arg(data[4*j])
                                          .arg(data[(4*j+1)])
                                          .arg(data[(4*j+2)])
                                          .arg(data[(4*j+3)]);
                        out <<  newLine;
                    }
                    outputfile.close();

                }
            }
     }
}

