#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "currentpositionshower.h"
#include "historyshower.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Chart *chart = new Chart;
    chart->init();
    chart->setTitle("Devices Quantity");
    chart->setAnimationOptions(QChart::AllAnimations);
    QChartView *chartView= new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->stackedWidget->insertWidget(0, chartView);
    ui->stackedWidget->setCurrentIndex(0);
    //ui->viewDevicesPositionGraph->test();
    ui->viewLongPeriodAnalysis->init();
    QObject::connect(ui->viewDevicesPositionGraph, SIGNAL(currentPositionClicked(QVector<QString>)),
                     ui->widgetDevicesPositionList, SLOT(currentPositionClickedHandler(QVector<QString>)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_DevicesQuantityButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
void MainWindow::on_DevicespositionButton_clicked()
{
    ui->viewDevicesPositionGraph->updateData();
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_buttonLongPeriodAnalysis_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_MovementsVisualizerButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_startAnimationButton_clicked()
{
    int duration=ui->sliderAnimationDuration->value();
    QString deviceId= ui->txtFieldDeviceId->text();
    ui->movementsShowerWindow->startAnimation(deviceId, duration);
}


void MainWindow::on_buttonLongPeriodAnalysisUpdate_clicked()
{
    int duration=ui->sliderLongPeriodAnalysis->value();
    ui->viewLongPeriodAnalysis->updateChart(static_cast<qulonglong>(duration));
}

void MainWindow::on_sliderLongPeriodAnalysis_valueChanged(int value)
{
    ui->labelLongPeriodAnalysis->setNum(value);
}

void MainWindow::on_sliderAnimationDuration_valueChanged(int value)
{
    ui->labelMovementVisualizerCounter->setNum(value);
}


void MainWindow::on_buttonDB_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery q(QDB::getInstance().getDB().get());
    q.prepare("SELECT * FROM packets");
    q.exec();
    model->setQuery(std::move(q));
    ui->viewDatabase->setModel(model);

}
