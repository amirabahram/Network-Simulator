#ifndef LINK_H
#define LINK_H

#include <QObject>
#include <memory>
class MovableNode;
class Link : public QObject
{
    Q_OBJECT
public:
    explicit Link(QObject *parent = nullptr);
    void setNodesMap(const std::unordered_map<int, MovableNode *> &newNodesMap);

public slots:
    void TransferData(std::shared_ptr<QByteArray> sharedByteArr, int rxId, int txId);
private:
    std::unordered_map<int,MovableNode*> nodesMap;
signals:
    void sendDataToNode(std::shared_ptr<QByteArray> sharedByteArr,const MovableNode& reciever,int senderId);
};

#endif // LINK_H
