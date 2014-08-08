#include "renderarea.h"
#include <QPainter>
#include <QTime>
#include <QPen>
#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include <math.h>
#include <assert.h>
RenderArea::RenderArea(QWidget *parent) :
    QWidget(parent)
{
//    qDebug() << "WIDTH + " << this->width() << "HEIGHT + " << this->height();
    scribbling = false;
    x[0] = 605*2/3;
    y[0] = 410*2/3;
    theta[0] = 3.1418/8;
    x[1] = 605/2;
    y[1] = 410/2;
    theta[1] = 0;
//    x[1] = y[1] = theta[1] = 0;
}

Pose RenderArea::getStartPose()
{
//    qDebug() << "Width height in startPose: " << this->width() << ", " << this->height();
    int xx = (x[0] * HALF_FIELD_MAXX * 2) / this->width() - HALF_FIELD_MAXX;
    int yy = -((y[0] * HALF_FIELD_MAXY * 2) / this->height() - HALF_FIELD_MAXY);
    return Pose(xx, yy, -theta[0]);
}

Pose RenderArea::getEndPose()
{
    int xx = (x[1] * HALF_FIELD_MAXX * 2) / this->width() - HALF_FIELD_MAXX;
    int yy = -((y[1] * HALF_FIELD_MAXY * 2) / this->height() - HALF_FIELD_MAXY);
    return Pose(xx, yy, -theta[1]);
}

void RenderArea::setStartPose(Pose p)
{
    x[0] = (p.queryX() + HALF_FIELD_MAXX) * this->width() / (2.0 * HALF_FIELD_MAXX);
    y[0] = (-p.queryY() + HALF_FIELD_MAXY) * this->height() / (2.0 * HALF_FIELD_MAXY);
    theta[0] = -p.queryTheta();
    update();
}

void RenderArea::setEndPose(Pose p)
{
    x[1] = (p.queryX() + HALF_FIELD_MAXX) * this->width() / (2.0 * HALF_FIELD_MAXX);
    y[1] = (-p.queryY() + HALF_FIELD_MAXY) * this->height() / (2.0 * HALF_FIELD_MAXY);
    theta[1] = -p.queryTheta();
    update();
}

void RenderArea::paintEvent(QPaintEvent *)
{
//    qDebug() << "Pose in render area: " << pose.x() << ", " << pose.y() << ", " << pose.theta()*180/3.141;
    QPainter painter(this);
    drawField(painter);
    drawBot(painter);    
    //drawPose(painter, 0);
    //drawPose(painter, 1);
}

void RenderArea::drawField(QPainter &painter)
{
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    QRect field;
    field.setTopLeft(QPoint(0,0));
    field.setBottomRight(QPoint(this->width()-1, this->height()-1));
    painter.setPen(Qt::black);
    painter.setBrush(QBrush(Qt::green));
    painter.drawRect(field);
    painter.restore();
}

void RenderArea::drawBot(QPainter &painter)
{
    // size of field in qt is 605x410, same ratio as in ssl-vision.
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    QRectF bot; // is in the strategy coordinate system only.
    bot.setHeight(223); // 7.5 * fieldXConvert, what would be better? since fieldXConvert != fieldYConvert, its approx
    bot.setWidth(223);
    bot.moveCenter(QPoint(0, 0));
    QLine midLine;
    midLine.setLine(bot.center().x(), bot.center().y(), bot.center().x()+bot.width()/2, bot.center().y());
    painter.translate(this->width()/2, this->height()/2);
    painter.scale(this->width()/(double)(2*HALF_FIELD_MAXX), this->height()/(double)(2*HALF_FIELD_MAXY));
    painter.translate(pose.queryX(), -pose.queryY());
    painter.rotate(-pose.queryTheta()*180/3.1415); // pathetic
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(4);
    painter.setPen(pen);
    painter.drawRect(bot);
    painter.drawLine(midLine);
    painter.restore();
}

void RenderArea::drawPose(QPainter &painter, int index)
{
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(2);
    painter.setPen(pen);
    int x2, y2;
    x2 = x[index] + cos(theta[index])*50;
    y2 = y[index] + sin(theta[index])*50;
    painter.drawLine(x[index], y[index], x2, y2);
    if(index)
        pen.setColor(Qt::red);
    else
        pen.setColor(Qt::blue);
    painter.setPen(pen);
    painter.drawEllipse(QPointF(x[index], y[index]), 10, 10);
    painter.restore();
}
