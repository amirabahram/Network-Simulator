#include "movingpoint.h"
#include <QBrush>

MovingPoint::MovingPoint(double x,double y,double dx,double dy)
{
        setRect(-5,-5,20,20);
        setBrush(Qt::blue);  //QBrush(QColor(212,175,55))
        setPos(x,y);
        direction = QPointF(dx,dy);
        setVisible(true);
}
void MovingPoint::move()
{
    setPos(pos()+direction*speedPixelsPerSecond);
    if(pos().x() < 0 || pos().x()>10000||pos().y()<0||pos().y()>10000){
        direction = -direction;
    }
}

void MovingPoint::setDirection(int d)
{

}

void MovingPoint::setSpeed(int v)
{
    speedKmh = v;
    speedPixelsPerSecond =(speedKmh * kmToPixel)/3600;
}
