#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QComboBox>
#include <QVBoxLayout>
#include <QtCharts/QChartView>
#include "chart.h"
#include "qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_DevicesQuantityButton_clicked();

    void on_MovementsVisualizerButton_clicked();

    void on_DevicespositionButton_clicked();

    void on_startAnimationButton_clicked();

    void on_buttonLongPeriodAnalysis_clicked();

    void on_buttonLongPeriodAnalysisUpdate_clicked();

    void on_sliderLongPeriodAnalysis_valueChanged(int value);

    void on_sliderAnimationDuration_valueChanged(int value);

    void on_buttonDB_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
