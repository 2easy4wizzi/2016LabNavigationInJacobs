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
// https://www.youtube.com/watch?v=tGKmQy-VBX0
#include <QTimer>
#include <QtMultimedia/qmediaplayer.h>
#include <QtMultimediaWidgets/qvideowidget.h>
#include <QGroupBox>
#include <QRadioButton>
#include <QCheckBox>



#include "dependencies/config.h"
#include "engine/includes.h"
#include "engine/edge.h"
#include "engine/node.h"
#include "engine/graph.h"

#define cout qDebug()<< __LINE__
#define xxx qDebug()<< __LINE__ ;
#define DEBUG 0

const QString fieldID = "Id";
const QString fieldNumber = "Number";
const QString fieldName = "Name";
const QString fieldNorth = "North";
const QString fieldEast = "East";
const QString fieldSouth = "South";
const QString fieldWest = "West";
const QString fieldStairs = "Stairs";
const QString fieldElevator = "Elevator";
const QString fieldFloor = "Floor";
const QString fieldSort = "Sort";


const QMap <Direction,QString> dirMap2 = { {  North, fieldNorth},{  East , fieldEast },{  South ,fieldSouth },{  West,fieldWest } };

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
    QString getRoomFieldById(int id, QString field);
    QStringList getRoomsTagsToPlaceInComboBox(QMap <int,QString> floorsToShow = QMap<int, QString>());
    void translateRoomsFromCppToQt();
    QList<pathRoomQt> translateShortestPathFromCppToQt();
    void printShortestPath(QList<pathRoomQt> shortestPathQt);
    void appendShortestPathToLog(QList<pathRoomQt> shortestPathQt);
    void playVideoFromTo(QString videoPath,int from, int to);
    void findWitchButtonIsOnPref();

public slots:
    void resetSlot();
    void currentLocationCbHasChangedSlot();
    void destinationCbHasChangedSlot();
    void goWasPressedSlot();
    void viewRoomsByHasChangedSlot();
    void prefWasChangedSlot();
    void showFilter1Slot();
    void showFilter2Slot();
    void updateFilter1Slot();
    void updateFilter2Slot();


signals:


private:
    void exitProgramWithErrMsg(QString errMsg);
    void showQmsgBox(QString msg);
    QList<QMap<QString,QString>> m_roomsObjects;//each room object got a list of attributes
    QVBoxLayout* m_mainVLayout;
    QPushButton* m_resetButton;
    QComboBox * m_currentLocationCb;
    QPushButton * m_currentLocationCbFilter;
    QGroupBox *m_groupBoxCurrentLocationCbWidget;
    QComboBox * m_destinationCb;
    QPushButton * m_destinationCbFilter;
    QGroupBox *m_groupBoxdestinationCbWidget;
    QPushButton* m_goButton;
    QTextEdit* m_log;
    Graph* m_graph;
    QMap<int, Node*> m_nodesMap;
    QString m_comboKey;
    Node* m_currentRoom;
    Node* m_destRoom;
    QHBoxLayout* m_titleLayout;
    QHBoxLayout* m_controlButtonsHLayout;
    QHBoxLayout* m_cbAndLabelsLayout;
    QHBoxLayout* m_logSpacerLayout;
    QMediaPlayer* m_mediaPlayer;
    QVideoWidget* m_videoWidget;
    QRadioButton *m_groupBoxPrefRadioDefault;
    QRadioButton *m_groupBoxPrefRadioStairs;
    QRadioButton *m_groupBoxPrefRadioElevator;
    QRadioButton *m_groupBoxViewByRadioName;
    QRadioButton *m_groupBoxViewByRadioNumber;
    EdgeType m_pref;
    QMap <int,QString> m_floorToShow1;
    QMap <int,QString> m_floorToShow2;
};

#endif // NAV_H
