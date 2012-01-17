#ifndef _DATA_PLOT_H
#define _DATA_PLOT_H 1

#include <qwt_plot.h>
#include "NIDAQmxBase.h"
#include <stdlib.h>
#include <QVector>

//Defaults
#define SCALE 1e9
#define HS_RES 10e6
#define LS_RES 10e3
#define TVS_SWITCH 6.0
#define TH_1 0.09
#define TH_2 0.05
#define TH_3 0.09
#define SAMPLES_PER_CHAN 1000
#define SAMPLE_RATE 10000

const int PLOT_SIZE = 1000;

class DataPlot : public QwtPlot
{
    Q_OBJECT

public:
    DataPlot(QWidget* = NULL);

public slots:
    void setTimerInterval(double interval);
    void logChannel(bool log_ai0);
    void inputfile(QString filename);
    void initDAQ();
    void quitDAQ();

    //Quad channel parameters
    void setScale(double);
    void setHSRes(double);
    void setLSRes(double);
    void setTVSLevel(double);
    void setTh1(double);
    void setTh2(double);
    void setTh3(double);

signals:
    void dataSample(QwtArray<double> data_sample);
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

    //Sample for histogram
    QwtArray<double> data_sample;

    int d_interval; // timer in ms
    int d_timerId;

    bool store_data;
    bool slope;
    bool read_data;

    //Quad channel amp. parameters
    double i_hs_res;
    double i_ls_res;
    double scale;
    double th_1, th_2, th_3;
    double tvs_switchover;

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
