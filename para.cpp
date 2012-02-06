/* Scanmaster-3000 parameter window
/
/  Copyright 2011, 2012 Doug Szumski <d.s.szumski@gmail.com>
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

#include "para.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QMessageBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QDebug>
#include <QChar>

para::para(QWidget* parent, Qt::WindowFlags flags): QDialog(parent, flags)
{
    QGridLayout* mainGrid = new QGridLayout;
    QVBoxLayout* topLayout = new QVBoxLayout;
    mainGrid->addLayout(topLayout,0,0);

    //Define Units
    QString ohms;
    ohms = (' ' + QChar(0x26, 0x21));

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
    QDoubleSpinBox* histStart= new QDoubleSpinBox();
    histStart->setMinimum(-10);
    //TODO: Link this to less than histstop
    histStart->setMaximum(10);
    histStart->setValue(DEF_XMIN);
    QLabel* histStartLabel = new QLabel(tr("Start position:"));
    hLayoutHistStart->addWidget(histStartLabel);
    hLayoutHistStart->addWidget(histStart);
    connect(histStart, SIGNAL(valueChanged(double)), this, SIGNAL(xMinPosition(double)));

    QHBoxLayout* hLayoutHistStop = new QHBoxLayout;
    QDoubleSpinBox* histStop = new QDoubleSpinBox();
    histStop->setValue(DEF_XMAX);
    histStop->setMaximum(10);
    QLabel* histStopLabel = new QLabel(tr("Stop position:"));
    hLayoutHistStop->addWidget(histStopLabel);
    hLayoutHistStop->addWidget(histStop);
    connect(histStop, SIGNAL(valueChanged(double)), this, SIGNAL(xMaxPosition(double)));

    //Quad channel parameters
    
    QHBoxLayout* hLayoutCurrentScale = new QHBoxLayout;
    QDoubleSpinBox* currentScale = new QDoubleSpinBox();
    currentScale->setMaximum(1000000000);
    currentScale->setValue(SCALE);
    currentScale->setDecimals(0);
    QLabel* currentScaleLabel = new QLabel(tr("Current scale factor:"));
    hLayoutCurrentScale->addWidget(currentScaleLabel);
    hLayoutCurrentScale->addWidget(currentScale);
    connect(currentScale, SIGNAL(valueChanged(double)), this, SIGNAL(scaleFactor(double)));

    QHBoxLayout* hLayoutHSRes = new QHBoxLayout;
    QDoubleSpinBox* HSRes = new QDoubleSpinBox();
    HSRes->setMaximum(1000000000);
    HSRes->setValue(HS_RES);
    HSRes->setDecimals(0);
    HSRes->setSuffix(ohms);
    QLabel* HSResLabel = new QLabel(tr("HS Resistance:"));
    hLayoutHSRes->addWidget(HSResLabel);
    hLayoutHSRes->addWidget(HSRes);
    connect(HSRes, SIGNAL(valueChanged(double)), this, SIGNAL(HSRes(double)));

    QHBoxLayout* hLayoutLSRes = new QHBoxLayout;
    QDoubleSpinBox* LSRes = new QDoubleSpinBox();
    LSRes->setMaximum(1000000);
    LSRes->setValue(LS_RES);
    LSRes->setDecimals(0);
    LSRes->setSuffix(ohms);
    QLabel* LSResLabel = new QLabel(tr("LS Resistance:"));
    hLayoutLSRes->addWidget(LSResLabel);
    hLayoutLSRes->addWidget(LSRes);
    connect(LSRes, SIGNAL(valueChanged(double)), this, SIGNAL(LSRes(double)));

    QHBoxLayout* hLayoutTVS = new QHBoxLayout;
    QDoubleSpinBox* TVS = new QDoubleSpinBox();
    TVS->setValue(TVS_SWITCH);
    TVS->setMaximum(10.0);
    QLabel* TVSLabel = new QLabel(tr("TVS threshold (V):"));
    hLayoutTVS->addWidget(TVSLabel);
    hLayoutTVS->addWidget(TVS);
    connect(TVS, SIGNAL(valueChanged(double)), this, SIGNAL(TVSLevel(double)));

    QHBoxLayout* hLayoutTH1 = new QHBoxLayout;
    QDoubleSpinBox* TH1 = new QDoubleSpinBox();
    TH1->setValue(TH_1);
    TH1->setMaximum(10.0);
    QLabel* TH1Label = new QLabel(tr("Threshold 1 (V):"));
    hLayoutTH1->addWidget(TH1Label);
    hLayoutTH1->addWidget(TH1);
    connect(TH1, SIGNAL(valueChanged(double)), this, SIGNAL(Th1(double)));

    QHBoxLayout* hLayoutTH2 = new QHBoxLayout;
    QDoubleSpinBox* TH2 = new QDoubleSpinBox();
    TH2->setValue(TH_2);
    TH2->setMaximum(10.0);
    QLabel* TH2Label = new QLabel(tr("Threshold 2 (V):"));
    hLayoutTH2->addWidget(TH2Label);
    hLayoutTH2->addWidget(TH2);
    connect(TH2, SIGNAL(valueChanged(double)), this, SIGNAL(Th2(double)));

    QHBoxLayout* hLayoutTH3 = new QHBoxLayout;
    QDoubleSpinBox* TH3 = new QDoubleSpinBox();
    TH3->setValue(TH_3);
    TH3->setMaximum(10.0);
    QLabel* TH3Label = new QLabel(tr("Threshold 3 (V):"));
    hLayoutTH3->addWidget(TH3Label);
    hLayoutTH3->addWidget(TH3);
    connect(TH3, SIGNAL(valueChanged(double)), this, SIGNAL(Th3(double)));

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

    setWindowTitle(tr("Settings"));

}

para::~para(){}
