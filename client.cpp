#include "client.h"
#include <QMetaEnum>
Client::Client(QObject *parent)
    : QObject{parent}
{
    _socket = new QTcpSocket(this);
    _socket->connectToHost("127.0.0.1",8080);
    connect(_socket,&QTcpSocket::connected,this,&Client::clientConnected);
    connect(_socket,&QTcpSocket::readyRead,this,&Client::isReadyRead);
    connect(_socket,&QTcpSocket::errorOccurred,this,&Client::ErrorHandler);
}

void Client::clientConnected()
{
    //qDebug()<<"Connected to server";
     emit messageToShow("Connected to server");
}

void Client::isReadyRead()
{

}

void Client::ErrorHandler(QAbstractSocket::SocketError error)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<QAbstractSocket::SocketError>();
    //qDebug()<<metaEnum.valueToKey(error);
    emit messageToShow(metaEnum.valueToKey(error));
}

void Client::writeDataToSocket(const QByteArray &d)
{
    _socket->write(d);
}

void Client::connectToServer()
{
    if(_socket->isOpen()){
        _socket->close();
        _socket->connectToHost("127.0.0.1",8080);
        emit messageToShow("reNew Connection to server");
        return;
    }
    _socket->connectToHost("127.0.0.1",8080);
    emit messageToShow("New Connection to server");
}


