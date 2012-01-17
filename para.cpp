#include "para.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QMessageBox>
#include <QSpinBox>
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
    connect(histBins, SIGNAL(valueChanged(int)), this, SLOT(numIntervals(int)));

    QHBoxLayout* hLayoutHistStart = new QHBoxLayout;
    QSpinBox* histStart= new QSpinBox();
    histStart->setMinimum(-10);
    //TODO: Link this to less than histstop
    histStart->setMaximum(10);
    histStart->setValue(DEF_XMIN);
    QLabel* histStartLabel = new QLabel(tr("Start position:"));
    hLayoutHistStart->addWidget(histStartLabel);
    hLayoutHistStart->addWidget(histStart);
    connect(histStart, SIGNAL(valueChanged(int)), this, SLOT(xMinPos(int)));

    QHBoxLayout* hLayoutHistStop = new QHBoxLayout;
    QSpinBox* histStop = new QSpinBox();
    histStop->setValue(DEF_XMAX);
    histStop->setMaximum(10);
    QLabel* histStopLabel = new QLabel(tr("Stop position:"));
    hLayoutHistStop->addWidget(histStopLabel);
    hLayoutHistStop->addWidget(histStop);
    connect(histStop, SIGNAL(valueChanged(int)), this, SLOT(xMaxPos(int)));

    mainGrid->addLayout(hLayoutHistBins,0,0);
    mainGrid->addLayout(hLayoutHistStart,1,0);
    mainGrid->addLayout(hLayoutHistStop,2,0);

    setLayout(mainGrid);


}

para::~para(){}

void para::numIntervals(int intervals){
    qDebug("Intervals changed to: %i", intervals);
    emit(intervalCount(intervals));
}

void para::xMinPos(int newxMinPos){
    qDebug("x-minpos changed to: %i", newxMinPos);
    emit(xMinPosition(newxMinPos));
}

void para::xMaxPos(int newxMaxPos){
    qDebug("x-maxpos changed to: %i", newxMaxPos);
    emit(xMaxPosition(newxMaxPos));
}

void para::sl_cancel(){
    QMessageBox msgBox;
    msgBox.setText("Cancel Clicked!");
    msgBox.exec();
}

