/* Scanmaster-3000
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

#include <QtGui>
#include "main.h"
#include "data_plot.h"
#include "hist_plot.h"
#include "para.h"
#include <QFileDialog>
#include <QSpinBox>
#include <QDialog>

MainWindow::MainWindow()
{
    setWindowTitle(tr("Scanmaster 3000"));

    //Plot graph
    DataPlot *dataplot = new DataPlot(this);

#if 0
    dataplot->scale = 1e9;
    dataplot->i_hs_res = 10e6;
    dataplot->i_ls_res = 10e3;
    dataplot->tvs_switchover = 6.0;
    dataplot->th_1 = 0.09;
    dataplot->th_2 = 0.05;
    dataplot->th_3 = 0.09;
#endif

    //Plot histogram
    HistPlot *histplot = new HistPlot(this);
    statusBar()->showMessage("QSimulate has started");

    //File menu
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setStatusTip(tr("Exit DataPlot"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
    //TODO FIX THE PROBLEM HERE WITH QTIMER GOING CRAZY AND PREVENTING LOADING
    setFileAct = new QAction(tr("Save DAQ output as..."), this);
    setFileAct->setStatusTip("Set filename to save DAQ output to");
    connect(setFileAct, SIGNAL(triggered()), this, SLOT(save()));

    fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction(setFileAct);
    fileMenu->addAction(exitAct);

    //DAQ menu
    DAQStartAct = new QAction(tr("&Start DAQ"), this);
    DAQStartAct->setStatusTip(tr("Start acquisition"));
    connect(DAQStartAct, SIGNAL(triggered()), dataplot, SLOT(initDAQ()));
    DAQStopAct= new QAction(tr("Sto&p DAQ"), this);
    DAQStopAct->setStatusTip(tr("Stop acquisition"));
    connect(DAQStopAct, SIGNAL(triggered()), dataplot, SLOT(quitDAQ()));

    DAQAqAct = new QAction(tr("Sav&e data to file"), this);
    DAQAqAct->setCheckable(true);
    DAQAqAct->setChecked(false);
    DAQAqAct->setStatusTip(tr("Saves the data to the file specified"));
    connect(DAQAqAct, SIGNAL(toggled(bool)), dataplot, SLOT(logChannel(bool)));

    DAQMenu = menuBar()->addMenu("&DAQ");
    DAQMenu->addAction(DAQStartAct);
    DAQMenu->addAction(DAQStopAct);
    DAQMenu->addAction(DAQAqAct);

    //Plot menu
    clearPlotAct = new QAction(tr("&Clear histogram"), this);
    clearPlotAct->setStatusTip("Clear on-screen histogram");
    connect(clearPlotAct, SIGNAL(triggered()), histplot, SLOT(clearPlot()));

    setParametersAct = new QAction(tr("&Settings"), this);
    setParametersAct->setStatusTip("Settings for data plotting");
    connect(setParametersAct, SIGNAL(triggered()), SLOT(showpars()));

    plotMenu = menuBar()->addMenu("&Plotting");
    plotMenu->addAction(clearPlotAct);
    plotMenu->addAction(setParametersAct);

    //About menu FIXME
    aboutAct = new QAction(tr("&Version"), this);
    aboutAct->setStatusTip(tr("Display version"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(VersionMessage()));
    aboutMenu = menuBar()->addMenu(tr("&About"));
    aboutMenu->addAction(aboutAct);

    //Embed histogram and DAQ output in a container
    QGroupBox *groupBox = new QGroupBox();
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(dataplot);
    vbox->addWidget(histplot);
    groupBox->setLayout(vbox);
    setCentralWidget(groupBox);
    
    //Plot histogram
    connect(dataplot, SIGNAL(dataSample(QwtArray<double>)), histplot, SLOT(updatePlot(QwtArray<double>)));

    //Create an instance of the parameters menu
    setHistPara = new para(this);
    //setHistPara->show(); //for testing

    //Connect the parameters to the histogram plotter
    connect(setHistPara, SIGNAL(intervalCount(int)), histplot, SLOT(setIntervals(int)));
    //Connect the histogram x axis min/max values to the histogram
    connect (setHistPara, SIGNAL(xMinPosition(int)), histplot, SLOT(setxStart(int)));
    connect (setHistPara, SIGNAL(xMaxPosition(int)), histplot, SLOT(setxStop(int)));
    //Clear the histogram when the parameters are changed
    connect(setHistPara, SIGNAL(intervalCount(int)), histplot, SLOT(clearPlot()));
    connect(setHistPara, SIGNAL(xMinPosition(int)), histplot, SLOT(clearPlot()));
    connect(setHistPara, SIGNAL(xMaxPosition(int)), histplot, SLOT(clearPlot()));
    //Connect the save file dialog to datplot
    connect(this, SIGNAL(newFilename(QString)), dataplot, SLOT(inputfile(QString)));

    //Connect the Quad channel parameters
    connect(setHistPara, SIGNAL(scaleFactor(double)), dataplot, SLOT(setScale(double)));
    connect(setHistPara, SIGNAL(HSRes(double)), dataplot, SLOT(setHSRes(double)));
    connect(setHistPara, SIGNAL(LSRes(double)), dataplot, SLOT(setLSRes(double)));
    connect(setHistPara, SIGNAL(TVSLevel(double)), dataplot, SLOT(setTVSLevel(double)));
    connect(setHistPara, SIGNAL(Th1(double)), dataplot, SLOT(setTh1(double)));
    connect(setHistPara, SIGNAL(Th2(double)), dataplot, SLOT(setTh2(double)));
    connect(setHistPara, SIGNAL(Th3(double)), dataplot, SLOT(setTh3(double)));


}

void MainWindow::save(void)
 {
    QFileDialog dialog;
    QString filename = dialog.getSaveFileName(this, tr("Save DAQ data to file"),
                            "output.txt",
                            tr("Text (*.txt)"));
    emit newFilename(filename);
 }

void MainWindow::showpars(void)
{
    setHistPara->show();
}


void MainWindow::VersionMessage(void)
{
     QMessageBox msgBox;
     msgBox.setText("v0.50 by Doug Szumski {d.s.szumski@gmail.com}");
     msgBox.setWindowTitle(tr("About"));
     msgBox.exec();
}

int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    MainWindow mainWindow;
    mainWindow.resize(1200,800);
    mainWindow.show();

    return a.exec(); 
}

