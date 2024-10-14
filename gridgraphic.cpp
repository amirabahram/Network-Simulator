#include <QGraphicsScene>
#include <QPen>
#include <QTimer>
#include "gridgraphic.h"

#include "ui_gridgraphic.h"

GridGraphic::GridGraphic(QWidget *parent,int cellNumber)
    : QGraphicsView(parent),
    cellNumber(cellNumber),
     ui(new Ui::GridGraphic)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(Qt::black);
    setScene(scene);
    int gridSize = 1000;
    int cellSize = gridSize / cellNumber;
    QPen gridPen(Qt::red);
    for(int x = 0;x<= gridSize;x+=cellSize){
        scene->addLine(x,0,x,gridSize,gridPen); //vertical lines
    }
    for(int y = 0;y<= gridSize;y+=cellSize){
        scene->addLine(0,y,gridSize,y,gridPen); //horizontal lines
    }
    //scene->setSceneRect(0,0,gridSize,cellSize);
    // QVector<QPointF> points = {
    //     {100,100},
    //     {300,200},
    //     {500,700},
    //     {800,900}
    // };
    // QVector<QPoint> redpoints ={{2,3},{4,4},{6,1},{9,8}};
    // for(const QPoint &redpoints: points){
    //     int x = point.x() *cellSize;
    //     int y = point.y() *cellSize;
    //     QGraphicsEllipseItem* ellipse = scene->addEllipse(x-5,y-5,10,10,QPen(Qt::NoPen),QBrush(Qt::red));
    //     ellipse->setFlag(QGraphicsItem::ItemIsMovable);

    // }
    // int i = -1;
    // for(const QPointF& point:points){
    //     i++;
    //     double dx = (qrand()%200-100)/100.0;
    //     double dy = (qrand()%200-100)/100.0;

    //     // MovingPoint* movingPoint = new MovingPoint(point.x(),point.y(),dx,dy);
    //     // scene->addItem(movingPoint);
    //     // pointsMap.insert(i,movingPoint);
    //     // QTimer* timer = new QTimer(this);
    //     // connect(timer,&QTimer::timeout,movingPoint,&MovingPoint::move);
    //     // timer->start(1000/60);
    // }
}

GridGraphic::~GridGraphic()
{
    delete ui;
    delete scene;
}

void GridGraphic::setNodesForShow(std::unordered_map<int, MovableNode *> nodesMap)
{
    for(const auto& node:nodesMap){
        scene->addItem(node.second);
        QTimer* timer = new QTimer(this);
         connect(timer,&QTimer::timeout,node.second,&MovableNode::move);
        timer->start(1000/60);
         connect(this,&GridGraphic::destroyed,timer,&QTimer::deleteLater);
    }
}


