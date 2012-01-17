#ifndef PARA_H
#define PARA_H
#include <QDialog>
#include "hist_plot.h"
#include "data_plot.h"

class para : public QDialog
{
    Q_OBJECT
 public:
     para(QWidget * parent = 0, Qt::WindowFlags f = 0);
     ~para();
 signals:
     void intervalCount(int);
     void xMinPosition(int);
     void xMaxPosition(int);
     void scaleFactor(double);
     void HSRes(double);
     void LSRes(double);
     void TVSLevel(double);
     void Th1(double);
     void Th2(double);
     void Th3(double);

};

#endif // PARA_H
