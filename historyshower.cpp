#include "historyshower.h"

HistoryShower::HistoryShower()
{
    int i;
    for (i=0;i<HistoryShower::DEV_NUM; i++)
    {
        x.push_back(std::make_shared<QVector<double>>());
        y.push_back(std::make_shared<QVector<double>>());
        id.push_back("No Graph");
    }
    this->setInteractions(QCP::iRangeZoom);
}
HistoryShower::HistoryShower(QWidget* & wid):HistoryShower::QCustomPlot(wid)
{
    int i;
    for (i=0;i<HistoryShower::DEV_NUM; i++)
    {
        x.push_back(std::make_shared<QVector<double>>());
        y.push_back(std::make_shared<QVector<double>>());
        id.push_back("No Graph");
    }
    //this->setInteractions(QCP::iRangeZoom);
}
void HistoryShower::init()
{
    int i;
    for (i=0;i<HistoryShower::DEV_NUM; i++)
    {
        this->addGraph();
        this->graph(i)->setPen(QPen(static_cast<Qt::GlobalColor>(HistoryShower::getCoulor(i))));
        this->graph(i)->setData(*x[i], *y[i]);
        this->graph(i)->setName("No Graph");
        this->graph(i)->setLineStyle(QCPGraph::LineStyle::lsStepRight);
    }
    this->yAxis->setRange(0,10);
    plotLayout()->addElement(1, 0, &this->sublayout);
    sublayout.setMargins(QMargins(5, 0, 5, 5));
    sublayout.addElement(0, 0, this->legend);
    // change the fill order of the legend, so it's filled left to right in columns:
    legend->setFillOrder(QCPLegend::foColumnsFirst);
    // set legend's row stretch factor very small so it ends up with minimum height:
    plotLayout()->setRowStretchFactor(1, 0.001);
    this->legend->setVisible(true);
    this->yAxis->setVisible(false);
}

void HistoryShower::updateChart(qulonglong days)
{
    std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<qulonglong, std::ratio<1>>> timeNow=
            std::chrono::time_point_cast<std::chrono::duration<qulonglong, std::ratio<1>>> (std::chrono::system_clock::now());
    qulonglong since = timeNow.time_since_epoch().count()-days*24*60*60;
    auto res=QDB::getInstance().selectMostFrequent(since);
    int i=0;
    for(i=0;i<HistoryShower::DEV_NUM;i++)
    {
        x[i]->clear();
        y[i]->clear();
        for(qulonglong j=0; j<=days+1;j++)
        {
            x[i]->push_back(j);
            y[i]->push_back(0);
        }
        id[i]="";
        this->graph(i)->setName("No Graph");
    }
    i=-1;
    qulonglong oldId=0;
    if(res->size()>0)
    {
        oldId=std::get<0>(res->operator[](0))-1;
    }
    for (auto it1=res->begin(); it1!=res->end(); it1++)
    {
        if(oldId!=std::get<0>(*it1))
        {
            i++;
            oldId=std::get<0>(*it1);
            id[i]=QString::number(std::get<0>(*it1),16);
            this->graph(i)->setName(id[i]);
        }
       int day= static_cast<int>((std::get<1>(*it1)-since)/(24*60*60))+1;
       y[i]->operator[](day)=2*(i+1);
    }
    //double range=days;
    //this->xAxis->setRange(-range,0);
    this->xAxis->setRange(0,days);
    for(i=0;i<HistoryShower::DEV_NUM;i++)
    {
       this->graph(i)->setData(*x[i], *y[i]);
    }
    this->replot();
}
