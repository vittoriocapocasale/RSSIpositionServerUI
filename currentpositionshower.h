#ifndef CURRENTPOSITIONSHOWER_H
#define CURRENTPOSITIONSHOWER_H

#include <qcustomplot.h>
#include <qdb.h>
class CurrentPositionShower: public QCustomPlot
{
    Q_OBJECT
public:
    CurrentPositionShower();
    CurrentPositionShower(QWidget* & wid);
    void append(double x, double y, QString id);
    void test();
    void clear();
    void plot();
    void updateData();
    void mousePressEvent(QMouseEvent* event) override;
signals:
    void xValue(double x);
    void currentPositionClicked(QVector<QString> data);
private:
    QVector<double> x;
    QVector<double> y;
    QVector<QString> id;
};

#endif // CURRENTPOSITIONSHOWER_H
