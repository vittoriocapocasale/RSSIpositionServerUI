#ifndef QLISTWIDGETEXTENDED_H
#define QLISTWIDGETEXTENDED_H

#include <QListWidget>

class QListWidgetExtended : public QListWidget
{
    Q_OBJECT
public:
    QListWidgetExtended();
    QListWidgetExtended(QWidget* &);
public slots:
    void currentPositionClickedHandler(QVector<QString> data);
};

#endif // QLISTWIDGETEXTENDED_H
