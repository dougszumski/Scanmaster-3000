#ifndef PARA_H
#define PARA_H
#include <QDialog>

#include <QObject>

class para : public QDialog
{
    Q_OBJECT
 public:
     para(QWidget * parent = 0, Qt::WindowFlags f = 0);
     ~para();
 public slots:
     void numIntervals(int intervals);
     void xMinPos(int newxMinPos);
     void xMaxPos(int newxMaxPos);
     void sl_cancel();
 signals:
     void intervalCount(int);
     void xMinPosition(int);
     void xMaxPosition(int);
};

#endif // PARA_H
