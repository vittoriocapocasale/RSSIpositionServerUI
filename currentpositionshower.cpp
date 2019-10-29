#include "currentpositionshower.h"
#include <QDebug>
CurrentPositionShower::CurrentPositionShower(){
    this->addGraph();
    this->xAxis->setRange(-1, 1);
    this->yAxis->setRange(-1, 1);
    this->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    this->graph(0)->setLineStyle(QCPGraph::lsNone);
}

CurrentPositionShower::CurrentPositionShower(QWidget* & wid):CurrentPositionShower::QCustomPlot(wid){
    this->addGraph();
    this->xAxis->setRange(-1, 1);
    this->yAxis->setRange(-1, 1);
    this->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    this->graph(0)->setLineStyle(QCPGraph::lsNone);
}

void CurrentPositionShower::test()
{
    this->x.push_back(0.7);
    this->y.push_back(0.7);
    this->id.push_back("io");
    this->x.push_back(0.7);
    this->y.push_back(0.7);
    this->id.push_back("io");
    this->x.push_back(0);
    this->y.push_back(0);
    this->id.push_back("ip");
    this->plot();
}

void CurrentPositionShower::plot()
{
    this->graph(0)->setData(x,y);
    this->replot();
    this->update();
}

void CurrentPositionShower::clear()
{
    x.clear();
    y.clear();
    id.clear();
}

void CurrentPositionShower::append(double x, double y, QString id)
{
    this->x.push_back(x);
    this->y.push_back(y);
    this->id.push_back(id);
}

void CurrentPositionShower::updateData(){
    auto data=QDB::getInstance().selectLastPositions();
    this->clear();
    for (auto a: *data)
    {
        this->append(std::get<1>(a),std::get<2>(a),QString::number(std::get<0>(a), 16) );
    }
    this->plot();
}

void CurrentPositionShower::mousePressEvent(QMouseEvent* event){
    double xValue=xAxis->pixelToCoord(event->pos().x());
    double yValue= yAxis->pixelToCoord(event->pos().y());
    int i;
    QVector<QString> data;
    for (i=0;i<x.size();i++)
    {
        if (fabs(x[i]-xValue)<0.05&&fabs(y[i]-yValue)<0.05)
        {
            QString s(id[i]);
            //QString s2=QString::number(x[i]);
            //QString s3=QString::number(y[i]);
            //s.append(" ").append(s2).append(" ").append(s3);
            data.push_back(s);
            //this->wid->addItem(s);
        }
    }
    emit currentPositionClicked(data);
}
