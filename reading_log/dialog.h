#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "pose.h"
#include <QTimer>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>
#include <deque>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>
#include <fstream>

#define NUMTICKS 300
using namespace std;
namespace Ui {
class Dialog;
}


struct RegData {
    double rho, gamma, delta;
    double timeMs;
    RegData():rho(0), gamma(0), delta(0), timeMs(0){}
    RegData(double rho, double gamma, double delta, double timeMs):rho(rho), gamma(gamma), delta(delta), timeMs(timeMs){}
};

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    
private slots:
    void on_startButton_clicked();

    void on_pauseButton_clicked();

    void on_resetButton_clicked();

    void on_horizontalSlider_sliderMoved(int position);

    void on_renderArea_destroyed();

    void on_tabWidget_destroyed();

public slots:
    void onCurIdxChanged(int idx); // idx is index of pose array, not botID (there's only 1 bot :/ )
    void onTimeout();
private:

    QTimer *algoTimer; //algoTimer for calling controller every 20ms. need to change to seperate thread.

    // don't need curIdx, simply read the position of the slider (otherwise there is duplicacy)
    Ui::Dialog *ui;
    QTimer *timer;

    void realtimeplot(int vl_prev,int vr_prev,int vl,int vr);

    QwtPlotGrid *grid;
    QwtPlotCurve *curve;

    QwtPlotGrid *grid_vr;
    QwtPlotCurve *curve_vr;

    ifstream file;
    FILE *fp;
};

#endif // DIALOG_H
