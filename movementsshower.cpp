#include "movementsshower.h"

MovementsShower::MovementsShower():textLabel(this)
{
    QObject::connect(&timer, &QTimer::timeout, this, &MovementsShower::showNext);
    timer.setInterval(1000);
    this->textLabel.setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
    this->textLabel.position->setType(QCPItemPosition::ptAxisRectRatio);
    this->textLabel.position->setCoords(0.5, 0);
    this->textLabel.setText("");
    this->addGraph();
    this->graph(0)->setData(x, y);
    this->xAxis->setRange(-1, 1);
    this->yAxis->setRange(-1, 1);
    this->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    this->graph(0)->setLineStyle(QCPGraph::lsNone);
    this->xIndex=0;
    this->replot();

}
MovementsShower::MovementsShower(QWidget* & q):QCustomPlot(q), textLabel(this){
    QObject::connect(&timer, &QTimer::timeout, this, &MovementsShower::showNext);
    timer.setInterval(1000);
    this->textLabel.setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
    this->textLabel.position->setType(QCPItemPosition::ptAxisRectRatio);
    this->textLabel.position->setCoords(0.5, 0);
    this->textLabel.setText("");
    this->addGraph();
    this->graph(0)->setData(x, y);
    this->xAxis->setRange(-1, 1);
    this->yAxis->setRange(-1, 1);
    this->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    this->graph(0)->setLineStyle(QCPGraph::lsNone);
    this->xIndex=0;
    this->replot();
}

void MovementsShower::startAnimation(QString deviceId, int duration)
{
    auto from=GlobalState::getInstance().getTime().time_since_epoch().count()-static_cast<unsigned int>(duration)*60;
    this->timer.stop();
    this->xIndex=0;
    this->allx.clear();
    this->ally.clear();
    this->alltimes.clear();
    bool ok;
    qulonglong id=deviceId.toULongLong(&ok, 16);
    if(!ok)
    {
        return;
    }
    auto r=QDB::getInstance().selectAllPositions(id, from);
    for(auto it=r->begin(); it!=r->end(); it++)
    {
        this->alltimes.push_back(std::get<0>(*it)-from);
        this->allx.push_back(std::get<1>(*it));
        this->ally.push_back(std::get<2>(*it));

    }
    this->showNext();
    this->timer.start();
}

void MovementsShower::showNext()
{
    this->x.clear();
    this->y.clear();
    if(this->xIndex<this->allx.length())
    {
        QString s=QString::number(this->alltimes.at(this->xIndex)/60);
        this->x.push_back(this->allx.at(this->xIndex));
        this->y.push_back(this->ally.at(this->xIndex));
        this->textLabel.setText("minute: "+s);
        this->xIndex++;
    }
    else {
        this->timer.stop();
        this->textLabel.setText("");
    }
    this->graph(0)->setData(x, y);
    this->update();
    this->replot();
}
