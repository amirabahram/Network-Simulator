#ifndef MOVABLENODE_H
#define MOVABLENODE_H

#include <QWidget>
#include <QGraphicsEllipseItem>
#include <QTimer>

namespace Ui {
class MovableNode;
}

class MovableNode : public QWidget,public QGraphicsEllipseItem

{
    Q_OBJECT
public:
    explicit MovableNode(QWidget *parent =nullptr);
    ~MovableNode();
    QWidget* getWidget();
void setNodeId(int newNodeId);
    static int s_mainNodeX;
    static int s_mainNodeY;
    void changeNodeColor();

public slots:
    void move();
    void Rx(std::shared_ptr<QByteArray> sharedByteArr, const MovableNode& n, int senderId);
    void Tx(const MovableNode& tx, int rxId, std::shared_ptr<QByteArray> sharedByteArr);



private:
    Ui::MovableNode *ui;
    QPointF direction;
    const double kmToPixel = 10.0;
    double speedKmh = 0.0;
    double speedPixelsPerSecond =(speedKmh * kmToPixel)/3600;
    int nodeId;
    int senderId;
    bool up = true;
    bool down = false;
    bool left = true;
    bool right = false;
    int nodeOffSet = 100;
    QByteArray buff;
    QTimer* timer;
    const int c_time = 1000;
    const int rate = 50; //bits
    bool isRecieving = false;
private slots:
    void setUp(bool newUp);

    void setDown(bool newDown);

    void setLeft(bool newLeft);

    void setRight(bool newRight);
    void setSpeed(int v);
    void plotData();
signals:
    void txData(std::shared_ptr<QByteArray> sharedByteArr,int rxId,int txId);
};

#endif // MOVABLENODE_H
