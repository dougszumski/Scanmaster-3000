#include "para.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QMessageBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QDebug>

para::para(QWidget* parent, Qt::WindowFlags flags): QDialog(parent, flags)
{
    QGridLayout* mainGrid = new QGridLayout;
    QVBoxLayout* topLayout = new QVBoxLayout;
    mainGrid->addLayout(topLayout,0,0);

    //Histogram bins
    QHBoxLayout* hLayoutHistBins = new QHBoxLayout;
    QSpinBox* histBins = new QSpinBox();
    histBins->setMaximum(1000);
    histBins->setValue(DEF_INTERVALS);
    QLabel* histBinsLabel = new QLabel(tr("Number of bins:"));
    hLayoutHistBins->addWidget(histBinsLabel);
    hLayoutHistBins->addWidget(histBins);
    connect(histBins, SIGNAL(valueChanged(int)), this, SIGNAL(intervalCount(int)));

    QHBoxLayout* hLayoutHistStart = new QHBoxLayout;
    QSpinBox* histStart= new QSpinBox();
    histStart->setMinimum(-10);
    //TODO: Link this to less than histstop
    histStart->setMaximum(10);
    histStart->setValue(DEF_XMIN);
    QLabel* histStartLabel = new QLabel(tr("Start position:"));
    hLayoutHistStart->addWidget(histStartLabel);
    hLayoutHistStart->addWidget(histStart);
    connect(histStart, SIGNAL(valueChanged(int)), this, SIGNAL(xMinPosition(int)));

    QHBoxLayout* hLayoutHistStop = new QHBoxLayout;
    QSpinBox* histStop = new QSpinBox();
    histStop->setValue(DEF_XMAX);
    histStop->setMaximum(10);
    QLabel* histStopLabel = new QLabel(tr("Stop position:"));
    hLayoutHistStop->addWidget(histStopLabel);
    hLayoutHistStop->addWidget(histStop);
    connect(histStop, SIGNAL(valueChanged(int)), this, SIGNAL(xMaxPosition(int)));

    //Quad channel parameters
    
    //Set current scaling  TODO -- convert this whole thing to a double so no need for faffing with the slot
    QHBoxLayout* hLayoutCurrentScale = new QHBoxLayout;
    QDoubleSpinBox* currentScale = new QDoubleSpinBox();
    currentScale->setMaximum(1e12);
    currentScale->setValue(SCALE);
    QLabel* currentScaleLabel = new QLabel(tr("Current scale factor:"));
    hLayoutCurrentScale->addWidget(currentScaleLabel);
    hLayoutCurrentScale->addWidget(currentScale);
    connect(currentScale, SIGNAL(valueChanged(double)), this, SLOT(scaleFac(double)));

    QHBoxLayout* hLayoutHSRes = new QHBoxLayout;
    QDoubleSpinBox* HSRes = new QDoubleSpinBox();
    HSRes->setValue(HS_RES);
    currentScale->setMaximum(1e12);
    QLabel* HSResLabel = new QLabel(tr("HS Resistance:"));
    hLayoutHSRes->addWidget(HSResLabel);
    hLayoutHSRes->addWidget(HSRes);

    QHBoxLayout* hLayoutLSRes = new QHBoxLayout;
    QDoubleSpinBox* LSRes = new QDoubleSpinBox();
    LSRes->setValue(LS_RES);
    currentScale->setMaximum(1e12);
    QLabel* LSResLabel = new QLabel(tr("LS Resistance:"));
    hLayoutLSRes->addWidget(LSResLabel);
    hLayoutLSRes->addWidget(LSRes);

    QHBoxLayout* hLayoutTVS = new QHBoxLayout;
    QDoubleSpinBox* TVS = new QDoubleSpinBox();
    TVS->setValue(TVS_SWITCH);
    currentScale->setMaximum(1e12);
    QLabel* TVSLabel = new QLabel(tr("TVS threshold (V):"));
    hLayoutTVS->addWidget(TVSLabel);
    hLayoutTVS->addWidget(TVS);

    QHBoxLayout* hLayoutTH1 = new QHBoxLayout;
    QDoubleSpinBox* TH1 = new QDoubleSpinBox();
    TH1->setValue(TH_1);
    currentScale->setMaximum(10.0);
    QLabel* TH1Label = new QLabel(tr("Threshold 1 (V):"));
    hLayoutTH1->addWidget(TH1Label);
    hLayoutTH1->addWidget(TH1);

    QHBoxLayout* hLayoutTH2 = new QHBoxLayout;
    QDoubleSpinBox* TH2 = new QDoubleSpinBox();
    TH2->setValue(TH_2);
    currentScale->setMaximum(10.0);
    QLabel* TH2Label = new QLabel(tr("Threshold 2 (V):"));
    hLayoutTH2->addWidget(TH2Label);
    hLayoutTH2->addWidget(TH2);

    QHBoxLayout* hLayoutTH3 = new QHBoxLayout;
    QDoubleSpinBox* TH3 = new QDoubleSpinBox();
    TH3->setValue(TH_3);
    currentScale->setMaximum(10.0);
    QLabel* TH3Label = new QLabel(tr("Threshold 3 (V):"));
    hLayoutTH3->addWidget(TH3Label);
    hLayoutTH3->addWidget(TH3);

    mainGrid->addLayout(hLayoutHistBins,0,0);
    mainGrid->addLayout(hLayoutHistStart,1,0);
    mainGrid->addLayout(hLayoutHistStop,2,0);
    mainGrid->addLayout(hLayoutCurrentScale,3,0);
    mainGrid->addLayout(hLayoutHSRes,4,0);
    mainGrid->addLayout(hLayoutLSRes,5,0);
    mainGrid->addLayout(hLayoutTVS,6,0);
    mainGrid->addLayout(hLayoutTH1,7,0);
    mainGrid->addLayout(hLayoutTH2,8,0);
    mainGrid->addLayout(hLayoutTH3,9,0);

    setLayout(mainGrid);


}

para::~para(){}

void para::scaleFac(double newScaleFac){
    emit(scaleFactor(newScaleFac));
}

