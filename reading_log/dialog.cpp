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
}

Dialog::~Dialog()
{
    fclose(fp);
    file.close();
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
}

void Dialog::on_horizontalSlider_sliderMoved(int )
{
    timer->stop();
}

void Dialog::onCurIdxChanged(int idx)
{
    double pos_x,pos_y,pos_theta;

    fscanf(fp,"%lf %lf %lf ",&pos_x,&pos_y,&pos_theta);
    qDebug() << "PosX: " << pos_x << " " << "PosY: " << pos_y << " " << "PosTheta: " << pos_theta;


    Pose pos(pos_x,pos_y,pos_theta);

    ui->renderArea->changePose(pos);

    fscanf(fp,"%lf %lf %lf",&pos_x,&pos_y,&pos_theta); // this is taking in the ball pos

}

void Dialog::realtimeplot(int vl_prev,int vr_prev,int vl,int vr){

    static qreal time = 0;
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

points << QPointF(time,vl_prev);
points_vr << QPointF(time,vr_prev);
time++;
points << QPointF(time,vl);
points_vr << QPointF(time,vr);

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


    int vrs, vls;

    fscanf(fp,"%d %d ",&vrs,&vls);
    fscanf(fp,"%d %d ",&vrs,&vls);


    qDebug() << "VLS: " << vls << " " << "VRS: " << vrs << " ";
    static int vl_prev = 0;
    static int vr_prev = 0;
    realtimeplot(vl_prev,vr_prev,vls,vrs);

    vl_prev = vls;
    vr_prev = vrs;


    ui->horizontalSlider->setValue(idx);
}

void Dialog::on_renderArea_destroyed()
{

}

void Dialog::on_tabWidget_destroyed()
{

}
