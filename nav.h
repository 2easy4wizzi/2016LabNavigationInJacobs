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
#include <QTimer>
#include <QtMultimedia/qmediaplayer.h>
#include <QtMultimediaWidgets/qvideowidget.h>
#include <QGroupBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QToolBar>



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
const QString fieldStairs = "Stairs";
const QString fieldElevator = "Elevator";
const QString fieldFloor = "Floor";

const QString fieldFromId = "fromId";
const QString fieldToId = "toId";
const QString fieldVideoStart = "videoStart";
const QString fieldVideoEnd = "videoEnd";
const QString fieldPathTovideo = "pathTovideo";



class Nav : public QWidget
{
    Q_OBJECT

public:
    explicit Nav(QWidget *parent = 0);
    ~Nav();
    void readRoomsFromXml();
    void initOnce();
    void init();
    Node* findNodeByStr(QString str);
    QString getRoomFieldById(int id, QString field);
    QStringList getRoomsTagsToPlaceInComboBox(QMap <int,QString> floorsToShow = QMap<int, QString>());
    void translateRoomsFromCppToQt();
    void translateShortestPathFromCppToQt();
    void printShortestPath();
    void appendShortestPathToLog(QString movieMessege, QString color = "black");
    void playVideoFromTo();
    void findWitchButtonIsOnPref();
    void testingFuncton();


public slots:
    void resetSlot();
    void currentLocationCbHasChangedSlot();
    void destinationCbHasChangedSlot();
    void goWasPressedSlot();
    void prefWasChangedSlot();
    void showFilter1Slot();
    void showFilter2Slot();
    void updateFilter1Slot();
    void updateFilter2Slot();
    void replaySlot();
    void nextSlot();
    void closeGroupBoxdestinationCbWidgetSlot();
    void closeGroupBoxCurrentLocationCbWidgetSlot();
    void stateOfMediaPlayerChangedSlot(QMediaPlayer::State state);
signals:


private:
    void exitProgramWithErrMsg(QString errMsg);
    void showQmsgBox(QString msg);
    QList<QMap<QString,QString>> m_roomsObjects;//each room object got a list of attributes
    //QMap<QPair<int,int> ,videoInfo> m_roomsEdges;//each edge key is QPair(fromId,ToId). each value is a videoInfo with startIndex, endIndex and videoPath.
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
    Node* m_currentRoom;
    Node* m_destRoom;
    QHBoxLayout* m_titleLayout;
    QHBoxLayout* m_controlButtonsHLayout;
    QHBoxLayout* m_cbAndLabelsLayout;
    QHBoxLayout* m_logSpacerLayout;
    QMediaPlayer* m_mediaPlayer;
    QVideoWidget* m_videoWidget;
    QMediaPlaylist* m_playlist;
    QRadioButton *m_groupBoxPrefRadioDefault;
    QRadioButton *m_groupBoxPrefRadioStairs;
    QRadioButton *m_groupBoxPrefRadioElevator;
    QRadioButton *m_groupBoxViewByRadioName;
    QRadioButton *m_groupBoxViewByRadioNumber;
    EdgeType m_pref;
    QMap <int,QString> m_floorToShow1;
    QMap <int,QString> m_floorToShow2;
    Node* m_roomVideoDisplay;
    QList<Node*> m_shortestPathQt;
    bool m_replay;
    bool m_appendedToLog;
    bool m_doneWithThisNode;
    bool m_next;
    int m_videoPlayerCounter;
};


#endif // NAV_H
