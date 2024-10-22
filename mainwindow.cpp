#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "link.h"
#include "cmath"
#include <MapGrid.h>
#include <MapGridView.h>
#include <QMessageBox>


QPointF MovableNode::s_mainNodePos;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    link = new Link();
    tcp = new Client();
    connect(tcp,&Client::messageToShow,this,&MainWindow::messegeHandler);
    connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::showGrid);
    connect(ui->pb_NodesCount,SIGNAL(valueChanged(int)),this,SLOT(setNodes(int)));
    connect(ui->pb_send,&QPushButton::clicked,this,&MainWindow::sendDataHandler);
    //connect(ui->pb_setupGnd,&QPushButton::clicked,this,&MainWindow::setupGrndStation);
    connect(this,&MainWindow::sendDatatoSocket,tcp,&Client::writeDataToSocket);
    connect(ui->pb_Mapdata,&QPushButton::clicked,this,&MainWindow::sendNodesDataToSocket);
    connect(ui->pb_connect,&QPushButton::clicked,tcp,&Client::connectToServer);
    connect(ui->pb_mapGrdView,&QPushButton::clicked,this,&MainWindow::testGrid);
    connect(ui->sp_pixelLength,SIGNAL(valueChanged(int)),this,SLOT(setCellsPerSide(int)));
    connect(ui->sp_zoneSize,SIGNAL(valueChanged(int)),this,SLOT(setGrdSizeKm(int)));
    connect(ui->sp_Lat,SIGNAL(valueChanged(double)),this,SLOT(setMainLatitude(double)));
    connect(ui->sp_Long,SIGNAL(valueChanged(double)),this,SLOT(setMainLongitude(double)));
    calculateGridBoundaries();

}
void MainWindow::calculateGridBoundaries() {
    // Constants: Earth radius and degrees per km (approximate for latitude)
    const double earthRadiusKm = 6371.0;
    const double degreesPerKmLat = 1.0 / 111.32;  // Latitude: 1 degree ≈ 111.32 km

    // Calculate half the grid size in degrees (latitude and longitude)
    double halfGridSizeLat = (ui->sp_zoneSize->value() / 2.0) * degreesPerKmLat;

    // For longitude, the distance per degree depends on the latitude, so we adjust:
    double degreesPerKmLon = 1.0 / (111.32 * cos(centerLat * M_PI / 180.0));
    double halfGridSizeLon = (ui->sp_zoneSize->value() / 2.0) * degreesPerKmLon;

    // Calculate the boundaries of the grid
    minLat = centerLat - halfGridSizeLat;
    maxLat = centerLat + halfGridSizeLat;
    minLon = centerLon - halfGridSizeLon;
    maxLon = centerLon + halfGridSizeLon;
}

void MainWindow::setMainLatitude(double newMainLatitude)
{
    _mainLatitude = newMainLatitude;
}

void MainWindow::setMainLongitude(double newMainLongitude)
{
    _mainLongitude = newMainLongitude;
}

void MainWindow::setCellsPerSide(int newCellsPerSide)
{
    _cellsPerSide = newCellsPerSide;
}

void MainWindow::setGrdSizeKm(int newGrdSizeKm)
{
    _grdSizeKm = newGrdSizeKm;
}
MainWindow::~MainWindow()
{
    delete ui;
    delete link;
}

void MainWindow::testGrid()
{


    if(grrd){
        grrd->show();
        return;
    }

}

void MainWindow::gridViewConnectionSetup()
{

}

void MainWindow::sendNodesDataToSocket()
{
    if(!grrd){
        qDebug() << "not set a grid yet!";
        return;
    }
    QPointF p;
    QByteArray data;
    QDataStream stream(&data,QIODevice::WriteOnly);
    stream <<static_cast<quint32>(nodesMap.size());
    for(auto i : nodesMap){
        QGraphicsEllipseItem* item = dynamic_cast<QGraphicsEllipseItem*>(i.second);
        p=grrd->getLongitudeLatitude(item,centerLat,centerLon,_grdSizeKm,_cellsPerSide);
        stream <<p.x();
        stream << p.y();

    }
    emit sendDatatoSocket(data);
}
QPair<qreal, qreal>  MainWindow::mapToGeoCoordinates(qreal x, qreal y) {
    // Normalize x and y to a value between 0 and 1
    qreal normalizedX = (x - sceneMinX) / (sceneMaxX - sceneMinX);
    qreal normalizedY = (y - sceneMinY) / (sceneMaxY - sceneMinY);

    // Map normalized values to geographic coordinates
    qreal longitude = geoMinLongitude + normalizedX * (geoMaxLongitude - geoMinLongitude);
    qreal latitude = geoMaxLatitude - normalizedY * (geoMaxLatitude - geoMinLatitude); // Invert Y-axis

    return qMakePair(longitude, latitude);
}
void MainWindow::showGrid()
{
    if(grdView){
      grdView->show();
        return;
    }
    QMessageBox::warning(this,"Warning","Please select node number first!");


}

void MainWindow::setNodes(int num)
{
    if(contentLayout)
    {
        delete contentLayout;
    }
    if(nodesMap.size()>0){
        for(auto& n:nodesMap){
            delete n.second;
        }
        nodesMap.clear();
    }
    if(grrd){
        delete grrd;
    }
    grrd= new MapGrid(_mainLatitude,_mainLongitude, _grdSizeKm, _cellsPerSide);
    // if(grdView){
    //     delete grdView;
    // }
    // grdView = new GridGraphic(nullptr,ui->sp_zoneSize->value()/ui->sp_pixelLength->value());
    contentLayout = new QVBoxLayout(ui->widget);
    //setupGrndStation();

    for(int i=0;i<num;i++)
    {
        MovableNode* node = new MovableNode(ui->widget);
        node->setNodeId(i+1);
        nodesMap.insert(std::make_pair(i+1,node));
        contentLayout->addWidget(node,i,0);
        connect(node,&MovableNode::txData,link,&Link::TransferData);
        connect(link,&Link::sendDataToNode,node,&MovableNode::Rx);
        connect(this,&MainWindow::setNodeToSendCommand,node,&MovableNode::Tx);

    }
    ui->widget->setLayout(contentLayout);
    //grdView->setNodesForShow(nodesMap);
    grrd->setNodesForShow(nodesMap);
    link->setNodesMap(nodesMap);
    setLatAndLongBoundaries();
}

void MainWindow::setupGrndStation()
{
    MovableNode* node = new MovableNode(ui->widget);
    node->setNodeId(0);
    nodesMap.insert(std::make_pair(0,node));
    QPointF mainPoint(0,0);
    setOrigin(mainPoint);
    node->s_mainNodePos = mainPoint;
    node->setNodePosition(mainPoint);
    node->changeNodeColor();
    qDebug() << "main node created";

}
QPointF MainWindow::mapToGrid(double lon, double lat) {
    int x = (lon - minLon) / (maxLon - minLon) * gridWidthInPixels;
    int y = gridHeightInPixels - (lat - minLat) / (maxLat - minLat) * gridHeightInPixels;  // Invert y-axis
    return QPointF(x, y);
}

void MainWindow::setLatAndLongBoundaries()
{
    ///user Inputs
    totalCoverageInKm = ui->sp_zoneSize->value();
    pixelLengthInKm  =  ui->sp_pixelLength->value();
    centerLat = ui->sp_Lat->value();
    centerLon = ui->sp_Long->value();

    halfCoverageLat = totalCoverageInKm / 2.0 / 110.574;  // Approx. km to latitude
    halfCoverageLon = totalCoverageInKm / 2.0 / (111.320 * cos(centerLat * M_PI / 180));  // km to longitude

    minLat = centerLat - halfCoverageLat;
    maxLat = centerLat + halfCoverageLat;
    minLon = centerLon - halfCoverageLon;
    maxLon = centerLon + halfCoverageLon;
}
void MainWindow::sendDataHandler()
{
    int tx = ui->sp_Tx->value();
    QString text = ui->plainTextEdit->toPlainText();
    QByteArray data(text.toUtf8());
    std::shared_ptr<QByteArray> sharedByteArr = std::make_shared<QByteArray>();
    sharedByteArr->append(data);
    auto it = nodesMap.find(tx);
    if(it != nodesMap.end()){
    emit setNodeToSendCommand(*nodesMap.at(tx),ui->sp_Rx->value(),sharedByteArr);
    }
}

void MainWindow::messegeHandler(QString str)
{
    ui->listWidget->addItem(str);
}
QPointF MainWindow::geoToPoint(double lon, double lat) {
    double x = lat-minLat;
    double y = lon -minLon;
    return QPointF(x, y);
}
QPointF MainWindow::pointToGeo(QPointF point) {
    // Invert y-axis since Qt uses top-left as (0, 0) but latitude increases upwards
    double lon = minLon + (point.x() / gridWidthInPixels) * (maxLon - minLon);
    double lat = minLat + ((gridHeightInPixels - point.y()) / gridHeightInPixels) * (maxLat - minLat);

    return QPointF(lon, lat);
}

void MainWindow::setOrigin(QPointF &p)
{
    QPointF center = grdView->getCenterPoint();
    p.setX(center.x());
    p.setY(center.y());
}
