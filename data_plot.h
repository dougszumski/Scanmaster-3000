#ifndef _DATA_PLOT_H
#define _DATA_PLOT_H 1

#include <qwt_plot.h>
#include "NIDAQmxBase.h"
#include <stdlib.h>
#include <QVector>

const int PLOT_SIZE = 1000;      // 0 to 200

class DataPlot : public QwtPlot
{
    Q_OBJECT

public:
    DataPlot(QWidget* = NULL);

public slots:
    void setTimerInterval(double interval);
    void logChannel(bool log_ai0);
    void setTrigger(double trigger);
    void setSlope(bool rising_slope);
    void inputfile(QString filename);
    void initDAQ();
    void quitDAQ();
    void generator(double array[], int len);

signals:
    void dataSample(QwtArray<double> data_sample);  //NOTE -- this seemed faster a QVector???
    void dataRandom(double value);

protected:
    void timerEvent(QTimerEvent *e);

private:
    void alignScales();

    double d_x[PLOT_SIZE]; 
    double ai_0[PLOT_SIZE];
    double ai_1[PLOT_SIZE];
    double ai_2[PLOT_SIZE];
    double ai_3[PLOT_SIZE];
    double ai_combined[PLOT_SIZE];

    //Resistor values
    double i_hs_res;
    double i_ls_res;
    double scale;

    //Sample for histogram
    QwtArray<double> data_sample;

    int d_interval; // timer in ms
    int d_timerId;

    bool store_data;
    bool slope;
    bool read_data;

    QString localfile;

    double trigger_point;
    double local_grad;

    // ADC Task
    TaskHandle  taskHandle;
    // Channel parameters
    float64     min;
    float64     max;
    // Timing parameters
    uInt64      samplesPerChan;
    float64     sampleRate;
     // Triggering parameters
    uInt32      triggerSlope;
    float64     triggerLevel;

    // Data read parameters
    #define     bufferSize (uInt32)4000
    float64     data[bufferSize];
    int32       pointsToRead;
    int32       pointsRead;
    float64     timeout;

};

#endif
