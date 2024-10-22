#include <QGraphicsScene>
#include <QPen>
#include <QTimer>
#include "gridgraphic.h"

#include "ui_gridgraphic.h"

GridGraphic::GridGraphic(QWidget *parent, int numCellsX)
    : QGraphicsView(parent),
     ui(new Ui::GridGraphic)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(Qt::black);
    setScene(scene);
    int gridSize = 1000;
    int cellSize = gridSize / numCellsX;
    QPen gridPen(Qt::red);
    for(int x = 0;x<= gridSize;x+=cellSize){
        scene->addLine(x,0,x,gridSize,gridPen); //vertical lines
    }
    for(int y = 0;y<= gridSize;y+=cellSize){
        scene->addLine(0,y,gridSize,y,gridPen); //horizontal lines
    }

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

QPointF GridGraphic::getCenterPoint()
{
    return QPointF(this->scene->width()/2,this->scene->height()/2);
}


