#include "chart.h"
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QtCore/QRandomGenerator>
#include <QtCore/QDebug>
#include <QLegendMarker>
Chart::Chart(QGraphicsItem *parent, Qt::WindowFlags wFlags):
    QChart(QChart::ChartTypeCartesian, parent, wFlags),
    m_series(nullptr),
    l_series(nullptr),
    m_axisX(new QValueAxis()),
    m_axisXFake(new QValueAxis()),
    m_axisY(new QValueAxis()),
    l_axisY(new QValueAxis()),
    m_step(0),
    m_x(0),
    m_y(0),
    l_x(0),
    l_y(0),
    cb(5)
{
     m_series = new QSplineSeries(this);
     l_series = new QSplineSeries(this);
}

Chart::~Chart()
{
    delete m_series;
    delete m_axisX;
    delete m_axisY;
    delete l_axisY;
    delete m_axisXFake;
}

void Chart::init()
{
    QObject::connect(&StallingStation::getInstance(), &StallingStation::newDataAvailable, this, &Chart::handleTimeout);
    //m_timer.setInterval(60000);
    //m_timer.start();
    QPen green(Qt::green);
    green.setWidth(3);
    m_series->setPen(green);
    m_series->append(m_x, m_y);
    addSeries(m_series);
    addAxis(m_axisX,Qt::AlignBottom);
    addAxis(m_axisY,Qt::AlignRight);
    m_series->attachAxis(m_axisY);
    m_series->attachAxis(m_axisX);
    m_series->setPointLabelsVisible(false);
    m_axisX->setTickCount(5);
    m_axisX->setRange(-20, 0);
    m_axisY->setRange(0, 100);
    m_axisX->hide();
    m_axisXFake->setTickCount(5);
    m_axisXFake->setRange(-4, 0);
    addAxis(m_axisXFake,Qt::AlignBottom);

    QPen red(Qt::red);
    l_series->setPen(red);
    l_series->append(l_x, l_y);
    addSeries(l_series);
    addAxis(l_axisY,Qt::AlignLeft);
    l_series->attachAxis(l_axisY);
    l_series->attachAxis(m_axisX);
    l_axisY->setRange(0, 100);
    l_axisY->setTickCount(6);
    legend()->setVisible(true);
    legend()->setAlignment(Qt::AlignBottom);
    legend()->markers()[0]->setLabel("Devices");
    legend()->markers()[1]->setLabel("Unrecognized percentage");
}

void Chart::handleTimeout(qulonglong time)
{
    qreal x =plotArea().width() / 4;//(m_axisX->tickCount()-1);
    //qreal y = (m_axisX->max() - m_axisX->min()) / m_axisX->tickCount();
    m_x +=5;
    std::tuple<qulonglong, qulonglong, qulonglong> counts=QDB::getInstance().selectCounts(time);
    qulonglong totalCount=std::get<0>(counts);
    qulonglong fakeCount=std::get<1>(counts);
    qulonglong normalCount=std::get<2>(counts);
    double precision=0;
    qulonglong fakeRecognized=totalCount-fakeCount-normalCount;
    qulonglong den=fakeRecognized+fakeCount;
    if(den>0)
    {
        precision= 100*fakeCount/den;
    }
    //qDebug()<<"den"<<den;
    //qDebug()<<"precision"<<precision;
    m_y=normalCount;
    l_y = precision;
    this->cb.add(m_y);
    double maxYvalue=this->cb.getMax();
    this->m_axisY->setRange(0, std::ceil((maxYvalue+0.1*maxYvalue+1)/10)*10);
    m_series->append(m_x, m_y);
    l_series->append(m_x,l_y);

    scroll(x, 0);
}
