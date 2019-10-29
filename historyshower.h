#ifndef HISTORYSHOWER_H
#define HISTORYSHOWER_H

#include <qcustomplot.h>
#include <globalstate.h>
#include <qdb.h>
class HistoryShower: public QCustomPlot
{
    Q_OBJECT
public:
    static const int DEV_NUM=5;
    static unsigned int getCoulor(int index)
    {
        switch (index){
        case 0:
            return Qt::red;
        case 1:
            return  Qt::green;
        case 2:
            return Qt::yellow;
        case 3:
            return Qt::blue;
        case 4:
            return Qt::darkRed;
        default:
            return Qt::white;
        }
    }
    HistoryShower();
    HistoryShower(QWidget* &);
    void init();
    void updateChart(qulonglong since);
private:
    QVector<std::shared_ptr<QVector<double>>> x;
    QVector<std::shared_ptr<QVector<double>>> y;
    QVector<QString> id;
    QCPLayoutGrid sublayout;
};

#endif // HISTORYSHOWER_H
