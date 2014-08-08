#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include "pose.h"
#include <QMutex>
class RenderArea : public QWidget
{
    Q_OBJECT
    Pose pose; // pose of the robot to be drawn

public:
    explicit RenderArea(QWidget *parent = 0);
    void changePose(Pose p) {pose = p; update();}
    Pose getStartPose();
    Pose getEndPose();
    void setStartPose(Pose p);
    void setEndPose(Pose p);
protected:
    void paintEvent(QPaintEvent *);
    void drawField(QPainter &painter);
    void drawBot(QPainter &painter);
    void drawPose(QPainter &painter, int index); //index is 0 or 1, for start or end pose respectively.
    bool scribbling;
    int x[7], y[7];    //
    double theta[7];   // All in QWidget coordinate system. Needs to be converted to our coordinates.
    int idx;
signals:
    
public slots:
    
};

#endif // RENDERAREA_H
