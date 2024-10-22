#ifndef MAPGRID_H
#define MAPGRID_H

#include <QWidget>
#include <qgraphicsview.h>

namespace Ui {
class MapGrid;
}
class MovableNode;
class MapGrid : public QGraphicsView
{
    Q_OBJECT

public:
    explicit MapGrid(double centerLat, double centerLon, double gridSizeKm, int cellsPerSide, QWidget* parent=nullptr);
    ~MapGrid();
    std::pair<double, double> getLongitudeLatitude(QGraphicsEllipseItem* marker, double centerLat, double centerLon, double cellSizeKm, int cellsPerSide);
    void setNodesForShow(std::unordered_map<int, MovableNode *> nodesMap);
private:
    Ui::MapGrid *ui;
    QGraphicsScene* scene;
};

#endif // MAPGRID_H
