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

    setRect(-5,-5,20,20);
    setBrush(Qt::blue);  //QBrush(QColor(212,175,55))
    int random_x =  s_mainNodeX+ (rand() % nodeOffSet);
    int random_y =  s_mainNodeY+ (rand() % nodeOffSet);
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
    timer = new QTimer();
}

void MovableNode::move()
{
    if(!up && !down && !left && !right ){
        return;
    }
    if(up){
        direction.setY(-1);
    }
    if(down){
        direction.setY(1);
    }
    if(up && down){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<>dist(0,1);
        int random_num = dist(gen);
        direction.setY(-1);
        if(random_num){
            direction.setY(1);
        }
        qDebug()<<"UpandDown";
    }
    if(left){
        direction.setX(-1);
    }
    if(right){
        direction.setX(1);
    }
    if(left && right){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<>dist(0,1);
        int random_num = dist(gen);
        direction.setX(-1);
        if(random_num){
            direction.setX(1);
        }
        qDebug()<<"LeftRight";
    }
    setPos(QGraphicsEllipseItem::pos()+direction*speedPixelsPerSecond);
    //qDebug()<<Q_FUNC_INFO <<direction.x()*speedPixelsPerSecond<<direction.y()*speedPixelsPerSecond;
    //moveBy(direction.x()*speedPixelsPerSecond,direction.y()*speedPixelsPerSecond);
    // if(QGraphicsEllipseItem::pos().x() < 0 || QGraphicsEllipseItem::pos().x()>10000||QGraphicsEllipseItem::pos().y()<0||QGraphicsEllipseItem::pos().y()>10000){
    //     direction = -direction;
    // }
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
void MovableNode::Tx(const MovableNode &tx, int rxId,std::shared_ptr<QByteArray> data)
{
    if(this == &tx){
        emit txData(data,rxId,this->nodeId);
    }

}

void MovableNode::changeNodeColor()
{
setBrush(Qt::red);
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
