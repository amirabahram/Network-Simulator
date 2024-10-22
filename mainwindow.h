#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <GridGraphic.h>
#include <movablenode.h>
#include <QMainWindow>
#include <QVBoxLayout>
#include <Client.h>
#include <MapGrid.h>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
class Link;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void testGrid();




private:
    Ui::MainWindow *ui;
    GridGraphic* grdView = nullptr;
    MapGrid* grrd = nullptr;
    void gridViewConnectionSetup();
    int nodeNum = 5;
    std::unordered_map<int,MovableNode*> nodesMap;
    QPair<qreal, qreal> mapToGeoCoordinates(qreal x, qreal y);
    QVBoxLayout* contentLayout = nullptr;
    Link* link = nullptr;
    Client *tcp = nullptr;
    void sendNodesDataToSocket();
    // Define your scene boundaries in your application
    qreal sceneMinX = 0.0;  // Minimum x value in your scene
    qreal sceneMaxX = 1000.0; // Maximum x value in your scene
    qreal sceneMinY = 0.0;  // Minimum y value in your scene
    qreal sceneMaxY = 1000.0; // Maximum y value in your scene

    // Define the geographic bounds
    qreal geoMinLongitude = -180.0; // Minimum longitude
    qreal geoMaxLongitude = 180.0;  // Maximum longitude
    qreal geoMinLatitude = -90.0;   // Minimum latitude
    qreal geoMaxLatitude = 90.0;    // Maximum latitude
    QPointF mapToGrid(double lon, double lat);

    double totalCoverageInKm = -1;
    double pixelLengthInKm  = -1;
    double gridWidthInPixels = -1;
    double gridHeightInPixels = -1;
    double centerLat = 35.69;
    double centerLon = 51.38;

    double halfCoverageLat = -1;  // Approx. km to latitude
    double halfCoverageLon = -1;

    double minLat = -1;
    double maxLat = -1;
    double minLon = -1;
    double maxLon = -1;
    void setLatAndLongBoundaries();
    QPointF geoToPoint(double lon, double lat);
    QPointF pointToGeo(QPointF point);
    void setOrigin(QPointF& p);
    void calculateGridBoundaries();
    int _grdSizeKm=20;
    int _cellsPerSide=10;
    double _mainLongitude = 51.40434;
    double _mainLatitude = 35.715298;
private slots:
    void showGrid();
    void setNodes(int num);
    void setupGrndStation();
    void sendDataHandler();
    void messegeHandler(QString str);
    void setGrdSizeKm(int newGrdSizeKm);

    void setCellsPerSide(int newCellsPerSide);
    void setMainLongitude(double newMainLongitude);

    void setMainLatitude(double newMainLatitude);
signals:
    void setNodeToSendCommand(const MovableNode& tx,int rxId,std::shared_ptr<QByteArray> sharedByteArr);
    void sendDatatoSocket(const QByteArray& a);

};
#endif // MAINWINDOW_H
