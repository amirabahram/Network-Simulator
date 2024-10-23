#include "movablenode.h"
#include "ui_movablenode.h"
#include <QBrush>
#include <QDebug>
#include <random>
#include <QMessageBox>
MovableNode::MovableNode(QWidget *parent)
    :QWidget(parent),
    ui(new Ui::MovableNode)
{
    ui->setupUi(this);

    setRect(0,0,0.1,0.1);
    setBrush(Qt::red);
    int random_x =  s_mainNodePos.x()+ (rand() % nodeOffSet);
    int random_y =  s_mainNodePos.y()+ (rand() % nodeOffSet);
    setPos(random_x,random_y);
    direction = QPointF(0,0);
    QGraphicsEllipseItem::setVisible(true);
     ui->pb_right->setIcon(QIcon(":/new/prefix1/ui_icon/w_r.png"));
     ui->pb_left->setIcon(QIcon(":/new/prefix1/ui_icon/w_l.png"));
     ui->pb_up->setIcon(QIcon(":/new/prefix1/ui_icon/w_u.png"));
     ui->pb_down->setIcon(QIcon(":/new/prefix1/ui_icon/w_d.png"));
    connect(ui->spinBox,SIGNAL(valueChanged(int)),this,SLOT(setSpeed(int)));
    connect(ui->pb_up,&QPushButton::toggled,this,&MovableNode::setUp);
    connect(ui->pb_down,&QPushButton::toggled,this,&MovableNode::setDown);
    connect(ui->pb_left,&QPushButton::toggled,this,&MovableNode::setLeft);
    connect(ui->pb_right,&QPushButton::toggled,this,&MovableNode::setRight);
    //connect(ui->cb_Limit,&QCheckBox::toggled,this,&MovableNode::setLimit);
    timer = new QTimer();
}

void MovableNode::move()
{
    if (!up && !down && !left && !right) {
        return;
    }

    if (up) {
        direction.setY(-1);
    }
    if (down) {
        direction.setY(1);
    }
    if (up && down) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, 1);
        int random_num = dist(gen);
        direction.setY(random_num ? 1 : -1);
    }
    if (left) {
        direction.setX(-1);
    }
    if (right) {
        direction.setX(1);
    }
    if (left && right) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, 1);
        int random_num = dist(gen);
        direction.setX(random_num ? 1 : -1);
    }

    QPointF newPos = QGraphicsEllipseItem::pos() + direction * speedPixelsPerSecond;

    // Check if the new position exceeds the boundaries and correct it
    if (newPos.x() < minX) {
        newPos.setX(minX);  // Fix position at the boundary
        direction.setX(1);  // Reverse X direction
    }
    if (newPos.x() > maxX) {
        newPos.setX(maxX);  // Fix position at the boundary
        direction.setX(-1); // Reverse X direction
    }
    if (newPos.y() < minY) {
        newPos.setY(minY);  // Fix position at the boundary
        direction.setY(1);  // Reverse Y direction
    }
    if (newPos.y() > maxY) {
        newPos.setY(maxY);  // Fix position at the boundary
        direction.setY(-1); // Reverse Y direction
    }

    // Ensure movement continues after reaching boundaries
    if (direction.x() == 0 && direction.y() == 0) {
        // In case the direction resets to (0, 0), restore default movement
        direction.setX(1);
        direction.setY(1);
    }

    // Set the new position after ensuring it's within bounds
    setPos(newPos);
    qDebug()<<"x:"<<newPos.x()<<"y:"<<newPos.y();
}

void MovableNode::Rx(std::shared_ptr<QByteArray> ch,const MovableNode& n,int senderId)
{
    if(isRecieving){
     QMessageBox::warning(this,"Warning","Collision Detected!");
        return;
    }
    if(this==&n && !isRecieving){
        this->senderId = senderId;
        isRecieving = true;
        for(int i=0;i<ch->size();i+=2){
            if(!ch->mid(i,2).contains("\r\n")){
                buff.append(ch->mid(i,2));
            }
        }
        connect(timer,&QTimer::timeout,this,&MovableNode::plotData);
        timer->start(c_time);
    }
}
void MovableNode::plotData()
{
    qDebug()<<"Tx:"<<senderId;
    qDebug()<<"Rx:"<<nodeId;
    qDebug()<<"Data:";
    qDebug()<<buff.mid(0,std::min(rate,int(buff.size())));
    buff.remove(0,std::min(rate,int(buff.size())));
    if(!buff.size()){
    disconnect(timer,&QTimer::timeout,this,&MovableNode::plotData);
    isRecieving = false;
    }
}

void MovableNode::setLimit(bool b)
{
    _isLimited = b;
}
void MovableNode::Tx(const MovableNode &tx, int rxId,std::shared_ptr<QByteArray> data)
{
    if(this == &tx){
        emit txData(data,rxId,this->nodeId);
    }

}

void MovableNode::setCellsPerSideNode(int cellsPerSide)
{
    _cellsPerSide = cellsPerSide;
    maxX = _cellsPerSide * _grdSizeKm;
    maxY = _cellsPerSide * _grdSizeKm;
}
void MovableNode::setGridSizeNode(int grdSize)
{
    _grdSizeKm = grdSize;
    maxX = _cellsPerSide * _grdSizeKm;
    maxY = _cellsPerSide * _grdSizeKm;
}
void MovableNode::changeNodeColor()
{
    setBrush(Qt::blue);
}

QPointF MovableNode::getNodePosition()
{
    return QGraphicsEllipseItem::pos();

}

void MovableNode::setNodePosition(QPointF pos)
{
    QGraphicsItem::setPos(pos.x(),pos.y());
}

void MovableNode::setSpeed(int v)
{
    speedKmh = v;
    speedPixelsPerSecond =(speedKmh * kmToPixel)/3600;
}





void MovableNode::setRight(bool newRight)
{

    right = newRight;
    if(right){
        ui->pb_right->setIcon(QIcon(":/new/prefix1/ui_icon/b_r.png"));
        return;
    }
    ui->pb_right->setIcon(QIcon(":/new/prefix1/ui_icon/w_r.png"));
}

void MovableNode::setLeft(bool newLeft)
{
    left = newLeft;
    if(left){
        ui->pb_left->setIcon(QIcon(":/new/prefix1/ui_icon/b_l.png"));
        return;
    }
    ui->pb_left->setIcon(QIcon(":/new/prefix1/ui_icon/w_l.png"));
}

void MovableNode::setDown(bool newDown)
{
    down = newDown;
    if(down){
        ui->pb_down->setIcon(QIcon(":/new/prefix1/ui_icon/b_d.png"));
        return;
    }
    ui->pb_down->setIcon(QIcon(":/new/prefix1/ui_icon/w_d.png"));
}

void MovableNode::setUp(bool newUp)
{
    up = newUp;
    if(up){
        ui->pb_up->setIcon(QIcon(":/new/prefix1/ui_icon/b_u.png"));
        return;
    }
    ui->pb_up->setIcon(QIcon(":/new/prefix1/ui_icon/w_u.png"));
}

void MovableNode::setNodeId(int newNodeId)
{
    nodeId = newNodeId;
    ui->label->setText("Node "+QString::number(nodeId)+" Speed(km/h): ");
}


MovableNode::~MovableNode()
{
    delete ui;
}

QWidget *MovableNode::getWidget()
{
    return ui->main;
}
