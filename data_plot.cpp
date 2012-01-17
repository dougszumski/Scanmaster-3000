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

    setTimerInterval(100.0);

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
    samplesPerChan = 1000;
    sampleRate = 10000;
//#if 0
    //Setup the task - takes bloody ages
    DAQmxBaseCreateTask("",&taskHandle);
    //Enable all 4 channels
    DAQmxBaseCreateAIVoltageChan(taskHandle,"Dev1/ai0, Dev1/ai1, Dev1/ai2, Dev1/ai3","",DAQmx_Val_Cfg_Default,min,max,DAQmx_Val_Volts,NULL);
    DAQmxBaseCfgSampClkTiming(taskHandle,"OnboardClock",sampleRate,DAQmx_Val_Rising,DAQmx_Val_ContSamps,samplesPerChan);
    DAQmxBaseCfgInputBuffer(taskHandle,40000); //DMA buffer size, overides 'auto' which doesn't work
    DAQmxBaseStartTask(taskHandle);
    //#endif
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

void DataPlot::setSlope(bool rising_slope)
{
    slope = rising_slope;

}

void DataPlot::setTrigger(double trigger)
//For now just trigger off channel ai0 with no averaging
{
    trigger_point = trigger;
}

void DataPlot::inputfile(QString filename)
{
    localfile = filename;
}

void DataPlot::generator(double array[], int len)
{
    for ( int i = 0; i < len; i++ )
        {
           const double value = (rand()%100000)/10000.0;
           array[i] += value;
           emit dataRandom(value);
           qDebug("value = %f", value);
         }


}

void DataPlot::timerEvent(QTimerEvent *)
        //  Read ADC, update the graph and save the data
{
    //Function protype: int32 DAQmxBaseReadAnalogF64 (TaskHandle taskHandle, int32 numSampsPerChan, float64 timeout,
    //bool32 fillMode, float64 readArray[ ], //uInt32 arraySizeInSamps, int32 *sampsPerChanRead, bool32 *reserved);
    if (read_data == true) {

        //Read the data
        DAQmxBaseReadAnalogF64(taskHandle,pointsToRead,timeout,DAQmx_Val_GroupByScanNumber,data,bufferSize,&pointsRead,NULL);

            //Update the graph
            for (int i = (int)samplesPerChan-1; i >= 0 && i < (int)samplesPerChan; i--){
                ai_0[i] = data[(4*i)];
                //ai_0[i] = ( (rand()%100000)/10000.0 -5.0);
                data_sample[i] = ai_0[i];
                ai_1[i] = data[(4*i+1)];
                ai_2[i] = data[(4*i+2)];
                ai_3[i] = data[(4*i+3)];
            }
            replot();

            //TODO: Move to GUI
            bool sat_flag_hs1;
            bool sat_flag_hs10;
            double fin_avg_hs1;
            double fin_avg_hs10;
            float th_1, th_2, th_3, bcorfac;
            int begin, final_int;
            i_hs_res = 10e6;
            i_ls_res = 10e3;
            scale = 1e9;
            th_1 = 1.0;
            th_2 = 0.08;
            th_3 = 1.0;
            bcorfac = 0.2;

            //Now have a look at the background levels on the HS channels to see if they've saturated
            final_int = int(samplesPerChan * bcorfac);
            begin = int(samplesPerChan - final_int);
            //Calculate the absolute average of the background

            //qDebug("Begin position: %i", begin);

            fin_avg_hs1 = 0.0;
            fin_avg_hs10 = 0.0;
            for (uint i = begin; i < samplesPerChan; i++){
                fin_avg_hs1 += fabs(ai_2[i]);
                fin_avg_hs10 += fabs(ai_3[i]);}
            fin_avg_hs1 /= final_int;
            fin_avg_hs10 /= final_int;

            //qDebug("Final average: %d", fin_avg_hs1);

            //Above replaces this:
            //fin_avg_hs1 = abs( sum(ai_2[begin:samplesPerChan]) / final_int);
            //fin_avg_hs10 = abs( sum(ai_3[begin:samplesPerChan]) / final_int);

            //Now see if the channels have saturated
            //TODO Add the 6.0V here as a parameter in the GUI. It is a function of the limiter circuit.
            sat_flag_hs1 = false;
            sat_flag_hs10 = false;
            if (fin_avg_hs1 > 6.0){
                qDebug() << "WARNING: Channels HSx1 and HSx10 are saturated and have been ignored";
                sat_flag_hs1 = true;
                //It follows that HSx10 must have also saturated
                sat_flag_hs10 = true;}
            else if (fin_avg_hs10 > 6.0){
                sat_flag_hs10 = true;}

            //Begin by setting the combined channel to the the lowest resolution
            memcpy(ai_combined, ai_0, sizeof(ai_0));

            //qDebug("Ai_combined: %f, ai_0: %f, Size of: %i)", ai_combined[2], ai_0[2], sizeof(ai_0));

            //Attempt to improve the resolution by stiching the channels together
            for (int i = 0; i < (int)samplesPerChan; i++)
            {
                if (fabs(ai_combined[i]) < th_1 ){
                    //Output below threshold so try and replace it with a higher sensitivity measurement
                    if (fabs(ai_1[i]) > th_2){
                        //LSx10 channel is still in operation so use the measurment from there
                        ai_combined[i] = ai_1[i] / (i_ls_res * 10) * scale;}
                    else if ((fabs(ai_2[i]) > th_3) && !sat_flag_hs1){
                        //Limiter is off, HSx1 channel is in operation
                        ai_combined[i] = ai_2[i] / i_hs_res * scale; }
                    else if (!sat_flag_hs10) {
                        //If HSx1 is below threshold then use the HSx10 channel regardless
                        ai_combined[i] = ai_3[i] / (i_hs_res * 10) * scale;}
                    else if (!sat_flag_hs1) {
                        //If the above fails because HSx10 is saturated fall back to HSx1 as that's the best we have
                        ai_combined[i] = ai_2[i] / i_hs_res * scale;}
                    else {
                        //If both HSx1 and HSx10 channels are saturated then use LSx10
                        //NOTE: In this case the measurement is probably useless for anything but a break junction
                        ai_combined[i] = ai_1[i] / (i_ls_res * 10) * scale;}
                 }
                 else {
                    //LSx1 channel is fine so convert to current
                    ai_combined[i] = ai_combined[i] / (i_ls_res) * scale;
                 }
            }

            //qDebug("AFTER Ai_combined: %f, ai_0: %f, Size of: %i)", ai_combined[2], ai_0[2], sizeof(ai_0));
            
            //Copy data from array to QwtArray - is there someway of casting it or using memcpy?
            
            for (uint i = 0; i < samplesPerChan; i++){
                data_sample[i] = log10(ai_combined[i]);
                //ai_combined[i] = log10(ai_combined[i]);

            }
            
            //qDebug("LOG Ai_combined: %f, ai_0: %f, Size of: %i)", ai_combined[2], ai_0[2], sizeof(ai_0));

            //Send data to histogram
            emit dataSample(data_sample);

#if 0       //NOT ACTUALLY REQUIRED AT THIS LEVEL
            /*Now convert the individual channels to currents*/
            for (int i = 0; i < (int)samplePerChan; i++) {
                ai_0[i] = ai_0[i] / i_ls_res * scale;
                ai_1[i] = ai_1[i] / (i_ls_res * 10) * scale;
                if (!sat_flag_hs1) {
                    ai_2[i] = ai_2[i] / i_hs_res * scale;}
                if (!sat_flag_hs10) {
                    ai_3[i] = ai_3[i] / (i_hs_res * 10) * scale;}
            }
#endif
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

