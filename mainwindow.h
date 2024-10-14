#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <GridGraphic.h>
#include <movablenode.h>
#include <QMainWindow>
#include <QVBoxLayout>
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

private:
    Ui::MainWindow *ui;
    GridGraphic* grdView = nullptr;
    void gridViewConnectionSetup();
    int nodeNum = 5;
    std::unordered_map<int,MovableNode*> nodesMap;
    QVBoxLayout* contentLayout = nullptr;
    Link* link = nullptr;
private slots:
    void showGrid();
    void setNodes(int num);
    void setupGrndStation();
    void sendDataHandler();
signals:
    void setNodeToSendCommand(const MovableNode& tx,int rxId,std::shared_ptr<QByteArray> sharedByteArr);

};
#endif // MAINWINDOW_H
