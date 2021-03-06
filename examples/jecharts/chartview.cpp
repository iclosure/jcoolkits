#include "chartview.h"
#include "jecharts_inc.h"
#include <QtWidgets>

ChartView::ChartView(QWidget *parent)
    : QDialog(parent, Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint)
    , d_timerId(0)
{
    QVBoxLayout *vertLayoutMain = new QVBoxLayout(this);
    vertLayoutMain->setContentsMargins(2, 2, 2, 2);
    vertLayoutMain->setSpacing(3);

    d_chartView = new JEcharts::JChartView(this);
    d_chartView->setColumnCount(2);
    vertLayoutMain->addWidget(d_chartView);

    QPushButton *button1 = new QPushButton("test", this);
    vertLayoutMain->addWidget(button1);

    //
    for (int i = 0; i < 1; ++i) {
        d_charts.append(new JEcharts::JStepChart(this));
        d_chartView->appendChart(d_charts.last(), false);
    }
    d_chartView->updateLayout();

    //
    JEcharts::JChart *chart = d_charts.first();

    //
    connect(chart->view(), SIGNAL(loadFinished(bool)),
            this, SLOT(onLoadFinished(bool)));
    //
    connect(button1, &QPushButton::clicked, [&, chart](){
        chart->option()->setProperty("title", "Sample");
        //QMetaObject::invokeMethod(chart->option(), "addData",
        //                          Q_ARG(int, 0), Q_ARG(QVariant, qrand() % 2));
        if (d_timerId == 0) {
            d_timerId = startTimer(500);
        } else {
            killTimer(d_timerId);
            d_timerId = 0;
        }
    });
}

void ChartView::onLoadFinished(bool loaded)
{
    Q_UNUSED(loaded);
    JEcharts::JChart *chart = d_charts.first();
    chart->option()->setProperty("title", "Step Line");
    chart->option()->setProperty("animation", "true");
    chart->option()->setProperty("option", "{backgroundColor:new echarts.graphic."
                                           "RadialGradient(0.3, 0.3, 0.8, ["
                                           "{offset:0, color:'#f7f8fa'},"
                                           "{offset:1, color:'#cdd0d5'}"
                                           "])}");
}

void ChartView::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == d_timerId) {
        JEcharts::JChart *chart = d_charts.at(0);
        for (int i = 0; i < 3; ++i) {
            QMetaObject::invokeMethod(chart->option(), "leftShiftData",
                                      Q_ARG(int, i), Q_ARG(QVariant, (qrand() % 2) + i * 2));
        }
    } else {
        QDialog::timerEvent(e);
    }
}
