#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
public slots:
        void writeDataToSocket(const QByteArray& d);
    void connectToServer();
private:
    QTcpSocket* _socket;
    void clientConnected();
    void isReadyRead();
    
    void ErrorHandler(QAbstractSocket::SocketError error);
signals:
    void messageToShow(QString m);
};

#endif // CLIENT_H
