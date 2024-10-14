#include "link.h"
#include "movablenode.h"

Link::Link(QObject *parent)
    : QObject{parent}
{}

void Link::TransferData(std::shared_ptr<QByteArray> sharedByteArr,int rxId,int txId)
{
    emit sendDataToNode(sharedByteArr,*nodesMap.at(rxId),txId);
}

void Link::setNodesMap(const std::unordered_map<int, MovableNode *> &newNodesMap)
{
    nodesMap = newNodesMap;
}


