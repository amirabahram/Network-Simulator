#include "mapgrid.h"
#include "ui_mapgrid.h"

#include <MapGridView.h>
#include <MovableNode.h>
#include <QGraphicsEllipseItem>
#include <QPointF>
#include <cmath>

// Constants for Earth's radius and conversion (assuming a planar approximation for simplicity)
const double EARTH_RADIUS_KM = 6371.0;
const double KM_PER_DEGREE_LAT = 111.32;  // Approximation: 1 degree of latitude = 111.32

MapGrid::MapGrid(double centerLat, double centerLon, double gridSizeKm, int cellsPerSide, QWidget* parent)
    : QGraphicsView(parent)
    , ui(new Ui::MapGrid)
{
    ui->setupUi(this);
    // Create a scene
    scene = new QGraphicsScene(this);

    double cellSizeKm = gridSizeKm / cellsPerSide;

    // Set up the pen for the grid lines (thin red lines)
    QPen redPen(Qt::red);
    redPen.setWidth(0);

    // Add vertical lines to the scene
    for (int col = 0; col <= cellsPerSide; ++col) {
        double x = col * cellSizeKm;

        // Create a vertical line (from top to bottom of the grid)
        QGraphicsLineItem* verticalLine = new QGraphicsLineItem(x, 0, x, cellsPerSide * cellSizeKm);
        verticalLine->setPen(redPen);

        // Add the vertical line to the scene
        scene->addItem(verticalLine);
    }

    // Add horizontal lines to the scene
    for (int row = 0; row <= cellsPerSide; ++row) {
        double y = row * cellSizeKm;

        // Create a horizontal line (from left to right of the grid)
        QGraphicsLineItem* horizontalLine = new QGraphicsLineItem(0, y, cellsPerSide * cellSizeKm, y);
        horizontalLine->setPen(redPen);

        // Add the horizontal line to the scene
        scene->addItem(horizontalLine);
    }

    // Mark Tehran's position in the center of the grid
    // Calculate the center of the grid
    double centerX = (cellsPerSide / 2) * cellSizeKm;
    double centerY = (cellsPerSide / 2) * cellSizeKm;

    // Create a blue ellipse to represent Tehran's position
    QGraphicsEllipseItem* tehranMarker = new QGraphicsEllipseItem(centerX - 0.1, centerY - 0.1, 0.2, 0.2); // Very small circle
    tehranMarker->setBrush(QBrush(Qt::blue));
    tehranMarker->setPen(QPen(Qt::blue)); // Blue outline for the marker

    // Add the Tehran marker to the scene
    scene->addItem(tehranMarker);

    // Set the scene in the view
    this->setScene(scene);

    // Scale the view to fit the entire grid
    this->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    getLongitudeLatitude(tehranMarker,centerLat, centerLon, cellSizeKm, cellsPerSide);
}

MapGrid::~MapGrid()
{
    delete ui;
}

std::pair<double, double> MapGrid::getLongitudeLatitude(QGraphicsEllipseItem *marker, double centerLat, double centerLon, double cellSizeKm, int cellsPerSide)
{
    QPointF pos = marker->scenePos();

    // Calculate how far the marker is from the center in scene coordinates
    double centerX = (cellsPerSide / 2) * cellSizeKm;
    double centerY = (cellsPerSide / 2) * cellSizeKm;

    // Distance in km from center
    double deltaXKm = (pos.x() - centerX);
    double deltaYKm = (pos.y() - centerY);

    // Latitude change (directly proportional to deltaYKm)
    double deltaLat = deltaYKm / KM_PER_DEGREE_LAT;

    // Longitude change (depends on latitude and deltaXKm)
    double cosLat = cos(centerLat * M_PI / 180.0);  // Convert latitude to radians
    double kmPerDegreeLon = KM_PER_DEGREE_LAT * cosLat;  // Longitude changes are scaled by cos(latitude)
    double deltaLon = deltaXKm / kmPerDegreeLon;

    // Calculate new latitude and longitude
    double newLat = centerLat + deltaLat;
    double newLon = centerLon + deltaLon;

    return std::make_pair(newLat, newLon);
}
void MapGrid::setNodesForShow(std::unordered_map<int, MovableNode *> nodesMap)
{
    for(const auto& node:nodesMap){
        scene->addItem(node.second);
        QTimer* timer = new QTimer(this);
        connect(timer,&QTimer::timeout,node.second,&MovableNode::move);
        timer->start(1000/60);
        connect(this,&MapGrid::destroyed,timer,&QTimer::deleteLater);
    }
}
