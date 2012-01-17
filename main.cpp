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

    setParametersAct = new QAction(tr("&Set parameters"), this);
    setParametersAct->setStatusTip("Set some parameters");
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
    setHistPara = new para;
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
    //TODO add error catch for undefined file in dataplot
    connect(this, SIGNAL(newFilename(QString)), dataplot, SLOT(inputfile(QString)));

        //TODO:
        // * Really dataplot should only plot data -- refactor to do this, and have seperate NiDAQ class
        //   fired by QTimer?
        // * Code to splice the three channels back together and take the log?
        //   * At this stage would be easier to integrate chopper for 'realtime' reconstruction of scans
        //     however important to keep totally raw data --output seperately as a gzipped file?
        //        * This would also improve the histogram
        //        * Could plot all data at once, so user can see real time data, and buffered scan reconstructs
        // * Setup some buttons to allow the user to:
        //   * Limit the y-axis scale -- perhaps set off the central region?
        //   *
        //   * Clear the histogram and optionally the stored data -- fast detection and removal of crap data
        //   * Enable /Disable real time updates incase large dataset eats up too much RAM

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
     msgBox.setText("Version: 0.50, Author: Doug Szumski {d.s.szumski@gmail.com}");
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

