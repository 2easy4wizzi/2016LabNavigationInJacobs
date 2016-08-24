#ifndef NAV_H
#define NAV_H

#include <QWidget>
#include <QDebug>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QMessageBox>
#include <QLabel>
#include <QComboBox>
#include <QString>

#include "dependencies/config.h"
#include "engine/includes.h"
#include "engine/edge.h"
#include "engine/node.h"
#include "engine/graph.h"

#define cout qDebug()<< __LINE__
#define xxx qDebug()<< __LINE__ ;
#define DEBUG 0

const QMap <Direction,QString> dirMap2 = { {  North, "North"},{  East , "East" },{  South ,"South" },{  West,"West" } };

//return nodes from GetShortestpath() in this struct
struct pathRoomQt {
    Node* room;
    int distance;
    QString direction;
    int nextRoomInPathId;
};

class Nav : public QWidget
{
    Q_OBJECT

public:
    explicit Nav(QWidget *parent = 0);
    ~Nav();
    void readAndCache();
    void readRoomsFromXml();
    void initOnce();
    void init();
    Node* findNodeByStr(QString str);
    QString getRoomValueByIdAndComboKey(int id);
    QStringList getRoomsTagsToPlaceInComboBox();
    void translateRoomsFromCppToQt();
    void printRooms();
    QList<pathRoomQt> translateShortestPathFromCppToQt();
    void printShortestPath(QList<pathRoomQt> shortestPathQt);
    void appendShortestPathToLog(QList<pathRoomQt> shortestPathQt);


public slots:
    void resetSlot();
    void currentLocationCbHasChangedSlot();
    void destinationCbHasChangedSlot();
    void goWasPressedSlot();
    void viewRoomsCbHasChangedSlot();


signals:


private:
    void exitProgramWithErrMsg(QString errMsg);
    void showQmsgBox(QString msg);
    QList<QMap<QString,QString>> m_roomsObjects;//each room object got a list of attributes
    QVBoxLayout* m_mainVLayout;
    QPushButton* m_resetButton;
    QComboBox * m_currentLocationCb;
    QComboBox * m_destinationCb;
    QPushButton* m_goButton;
    QTextEdit* m_log;
    Graph* m_graph;
    QMap<int, Node*> m_nodesMap;
    QString m_comboKey;
    QComboBox * m_viewRoomsCb;
    Node* m_currentRoom;
    Node* m_destRoom;
    QHBoxLayout* m_titleLayout;
    QHBoxLayout* m_controlButtonsHLayout;
    QHBoxLayout* m_cbAndLabelsLayout;
    QHBoxLayout* m_logSpacerLayout;
};

#endif // NAV_H