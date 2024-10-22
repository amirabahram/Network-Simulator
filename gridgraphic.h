#ifndef GRIDGRAPHIC_H
#define GRIDGRAPHIC_H
#include "movingpoint.h"
#include <QWidget>
#include <QGraphicsView>
#include <MovableNode.h>
namespace Ui {
class GridGraphic;
}

class GridGraphic : public QGraphicsView
{
    Q_OBJECT

public:
    explicit GridGraphic(QWidget *parent = nullptr,int numCellsX=20);
    ~GridGraphic();
    void setNodesForShow(std::unordered_map<int,MovableNode*> nodesMap);
    QPointF getCenterPoint();
public slots:

private:
    Ui::GridGraphic *ui;
    QGraphicsScene* scene=nullptr;

};

#endif // GRIDGRAPHIC_H
