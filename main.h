#include <QMainWindow>
#include "para.h"

#ifndef MAIN_H
#define MAIN_H

class MainWindow: public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow();
    private slots:
        void VersionMessage(void);
        void save(void);
        void showpars(void);
    private:
        QMenu *fileMenu;
        QMenu *DAQMenu;
        QMenu *plotMenu;
        QMenu *aboutMenu;
        QAction *exitAct;
        QAction *setFileAct;
        QAction *DAQStartAct;
        QAction *DAQStopAct;
        QAction *DAQAqAct;
        QAction *clearPlotAct;
        QAction *aboutAct;
        QAction *setParametersAct;
        para *setHistPara;
    signals:
        void newFilename(QString);

};

#endif // MAIN_H
