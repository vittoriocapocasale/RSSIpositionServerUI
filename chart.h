#ifndef CHART_H
#define CHART_H

#include <QtCharts/QChart>
#include <QtCore/QTimer>
#include <qdb.h>
#include <circularbuffer.h>
#include <stallingstation.h>
#include <QLineSeries>
QT_CHARTS_BEGIN_NAMESPACE
class QSplineSeries;
class QValueAxis;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class Chart: public QChart
{
    Q_OBJECT
public:
    Chart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~Chart();
    Chart(const Chart& c)=delete;
    Chart& operator=(const Chart& c)=delete;
    void init();
public slots:
    void handleTimeout(qulonglong time);

private:
    //QTimer m_timer;
    QLineSeries *m_series;
    QLineSeries *l_series;
    QStringList m_titles;
    QValueAxis *m_axisX;
    QValueAxis *m_axisXFake;
    QValueAxis *m_axisY;
    QValueAxis *l_axisY;
    qreal m_step;
    qreal m_x;
    qreal m_y;
    qreal l_x;
    qreal l_y;
    int status;
    CircularBuffer<double> cb;
};

#endif /* CHART_H */
