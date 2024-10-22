#ifndef MAPGRIDVIEW_H
#define MAPGRIDVIEW_H

#include <QObject>
#include <qgraphicsview.h>

class MapGridView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MapGridView(double centerLat, double centerLon, double gridSizeKm, int cellsPerSide, QWidget* parent = nullptr);

signals:
};

#endif // MAPGRIDVIEW_H
