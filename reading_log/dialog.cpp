#include "dialog.h"
#include "ui_dialog.h"
#include "geometry.h"
#include <stdio.h>
#include <assert.h>
#include <QTimer>
#include <QDebug>
#include <algorithm>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_multifit.h>
#include <limits.h>
#include <limits>
#include <qapplication.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>

#define timeLCMs 16

using namespace std;
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{

    srand(time(NULL));
    ui->setupUi(this);
    timer = new QTimer();

    ui->horizontalSlider->setRange(0, NUMTICKS-1);
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(onCurIdxChanged(int)));
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));

    onCurIdxChanged(0);

    ui->plot->setTitle( "Left Velocity" );
    ui->plot->setCanvasBackground( Qt::white );
    ui->plot->setAxisScale( QwtPlot::yLeft, -200.0, 200.0 );
    ui->plot->setAxisScale( QwtPlot::xBottom, 0.0, 300.0 );
    ui->plot->insertLegend( new QwtLegend() );

    grid = new QwtPlotGrid();
    grid->attach( ui->plot );


    ui->plot_vr->setTitle( "Right Velocity" );
    ui->plot_vr->setCanvasBackground( Qt::white );
    ui->plot_vr->setAxisScale( QwtPlot::yLeft, -200.0, 200.0 );
    ui->plot_vr->setAxisScale( QwtPlot::xBottom, 0.0, 300.0 );
    ui->plot_vr->insertLegend( new QwtLegend() );

    grid_vr = new QwtPlotGrid();
    grid_vr->attach( ui->plot_vr );

    fp = fopen("data.log","r");
    if(fp == NULL){exit(1);}


    int index = 0;

    while(!feof(fp)){

        fscanf(fp,"%d %d ",&vrs_sent[index],&vls_sent[index]);
        fscanf(fp,"%d %d ",&vrs_recv[index],&vls_recv[index]);

        fscanf(fp,"%lf %lf %lf ",&bot_x[index],&bot_y[index],&bot_theta[index]);
        fscanf(fp,"%lf %lf %lf ",&ball_x[index],&ball_y[index],&ball_theta[index]);

        printf("%d %d %lf %lf %lf %lf %lf %lf",vrs_sent[index],vls_sent[index],bot_x[index],bot_y[index],bot_theta[index],
               ball_x[index],ball_y[index],ball_theta[index]);

        index++;

    }

    fclose(fp);
   // file.close();
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_startButton_clicked()
{
    timer->start(timeLCMs);
}

void Dialog::on_pauseButton_clicked()
{
    timer->stop();
}

void Dialog::on_resetButton_clicked()
{
    timer->stop();
    ui->horizontalSlider->setValue(0);

   ui->plot->QwtPlot::detachItems(QwtPlotItem::Rtti_PlotItem, true);
   ui->plot->replot();

   ui->plot_vr->QwtPlot::detachItems(QwtPlotItem::Rtti_PlotItem, true);
   ui->plot_vr->replot();
}

void Dialog::on_horizontalSlider_sliderMoved(int )
{
    timer->stop();
}

void Dialog::onCurIdxChanged(int idx)
{
    Pose pos(bot_x[idx],bot_y[idx],bot_theta[idx]);
    ui->renderArea->changePose(pos);
}

void Dialog::realtimeplot(int vl_prev,int vr_prev,int vl,int vr,int idx){

    curve = new QwtPlotCurve();
    curve_vr = new QwtPlotCurve();
    curve->setTitle( "Some Points" );
    curve_vr->setTitle( " Some Points ");
    QPen penplot(Qt::blue,2);
    QPen penplot_vr(Qt::red,2);

     curve->setPen( penplot );
     curve->setRenderHint( QwtPlotItem::RenderAntialiased, true );


     curve_vr->setPen( penplot_vr );
     curve_vr->setRenderHint( QwtPlotItem::RenderAntialiased, true );


    QPolygonF points;
    QPolygonF points_vr;

/* points << QPointF( 0.0, 4.4 ) << QPointF( 1.0, 3.0 )
    << QPointF( 2.0, 4.5 ) << QPointF( 3.0, 6.8 )
    << QPointF( 4.0, 7.9 ) << QPointF( 5.0, 7.1 );*/

points << QPointF(idx-1,vl_prev);
points_vr << QPointF(idx-1,vr_prev);

points << QPointF(idx,vl);
points_vr << QPointF(idx,vr);

curve->setData( points );
curve_vr->setData( points_vr);

curve->attach( ui->plot );
curve_vr->attach( ui->plot_vr );

ui->plot->resize( 600, 400 );
ui->plot->show();

ui->plot_vr->resize( 600, 400 );
ui->plot_vr->show();

}

void Dialog::onTimeout()
{
    int idx = ui->horizontalSlider->value();

    idx++;

    if(idx >= NUMTICKS) {
        timer->stop();
        return;
    }
    if(idx < 0 || idx >= NUMTICKS) {
        qDebug() << "Error! idx = " << idx << " and is out of range!";
        return;
    }


    realtimeplot(vls_sent[idx-1],vrs_sent[idx-1],vls_sent[idx],vrs_sent[idx],idx);

    ui->horizontalSlider->setValue(idx);
}

void Dialog::on_renderArea_destroyed()
{

}

void Dialog::on_tabWidget_destroyed()
{

}
