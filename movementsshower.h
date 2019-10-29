#ifndef MOVEMENTSSHOWER_H
#define MOVEMENTSSHOWER_H
#include <qcustomplot.h>
#include <qdb.h>
#include <globalstate.h>
class MovementsShower: public QCustomPlot
{
    Q_OBJECT
public:
    MovementsShower();
    MovementsShower(QWidget* & q);
    void startAnimation(QString deviceId, int duration);
public slots:
    void showNext();
private:
    QCPItemText textLabel;
    QTimer timer;
    QVector<double> x;
    QVector<double> y;
    QVector<double> allx;
    QVector<double> ally;
    QVector<qulonglong> alltimes;
    int xIndex;
    qulonglong currentTime;
};

#endif // MOVEMENTSSHOWER_H
