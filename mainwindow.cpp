#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "link.h"

#include <QMessageBox>


int MovableNode::s_mainNodeX;
int MovableNode::s_mainNodeY;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    link = new Link();
    connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::showGrid);
    connect(ui->pb_nodeNum,SIGNAL(valueChanged(int)),this,SLOT(setNodes(int)));
    connect(ui->pb_send,&QPushButton::clicked,this,&MainWindow::sendDataHandler);
    //connect(ui->pb_setupGnd,&QPushButton::clicked,this,&MainWindow::setupGrndStation);


}

MainWindow::~MainWindow()
{
    delete ui;
    delete link;
}

void MainWindow::gridViewConnectionSetup()
{

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
    setupGrndStation();
    if(grdView){
        delete grdView;
    }
    contentLayout = new QVBoxLayout(ui->widget);
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
    grdView = new GridGraphic(nullptr,ui->sp_cell->value());
    grdView->setNodesForShow(nodesMap);
    link->setNodesMap(nodesMap);
}

void MainWindow::setupGrndStation()
{
  MovableNode* node = new MovableNode(ui->widget);
    node->setNodeId(0);
    nodesMap.insert(std::make_pair(0,node));
    node->s_mainNodeX = ui->sp_X->value();
    node->s_mainNodeY = ui->sp_Y->value();
    node->changeNodeColor();
    qDebug() << "main node created";

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
