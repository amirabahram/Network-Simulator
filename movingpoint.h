#ifndef MOVINGPOINT_H
#define MOVINGPOINT_H

#include <QGraphicsEllipseItem>
#include <QObject>


class MovingPoint : public QObject,
                    public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    explicit MovingPoint(double x,double y,double dx,double dy);
public slots:
    void move();
    void setDirection(int d);
    void setSpeed(int v);
private:
    QPointF direction;
    const double kmToPixel = 10.0;
    double speedKmh = 10.0;
    double speedPixelsPerSecond =(speedKmh * kmToPixel)/3600;

signals:
};

#endif // MOVINGPOINT_H
