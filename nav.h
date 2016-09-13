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
#include <QtXml>
#include <QFile>

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

struct configVars{


configVars(QMap<QString,QString> configHeaderToValue)
{
    fontType = configHeaderToValue.contains("fontType") ? configHeaderToValue["fontType"] : "";
    fontSize = configHeaderToValue.contains("fontSize") ? configHeaderToValue["fontSize"] : "";
    border = configHeaderToValue.contains("border") ? configHeaderToValue["border"] : "";
    globalTextAttributes = fontType + fontSize + border;
    roomsXmlPath = configHeaderToValue.contains("roomsXmlPath") ? configHeaderToValue["roomsXmlPath"] : "";

    projectLogo = configHeaderToValue.contains("projectLogo") ? configHeaderToValue["projectLogo"] : "";
    projectTitleLabelText = configHeaderToValue.contains("projectTitleLabelText") ? configHeaderToValue["projectTitleLabelText"] : "";
    projectTitleLabelStyle = configHeaderToValue.contains("projectTitleLabelStyle") ? configHeaderToValue["projectTitleLabelStyle"] : "";
    windowTitleText = configHeaderToValue.contains("windowTitleText") ? configHeaderToValue["windowTitleText"] : "";
    currentLocationLabelText = configHeaderToValue.contains("currentLocationLabelText") ? configHeaderToValue["currentLocationLabelText"] : "";

    currentLocationLabelStyle = configHeaderToValue.contains("currentLocationLabelStyle") ? configHeaderToValue["currentLocationLabelStyle"] : "";
    destinationLabelText = configHeaderToValue.contains("destinationLabelText") ? configHeaderToValue["destinationLabelText"] : "";
    destinationLabelStyle = configHeaderToValue.contains("destinationLabelStyle") ? configHeaderToValue["destinationLabelStyle"] : "";
    logLabelText = configHeaderToValue.contains("logLabelText") ? configHeaderToValue["logLabelText"] : "";
    logLabelStyle = configHeaderToValue.contains("logLabelStyle") ? configHeaderToValue["logLabelStyle"] : "";

    logStyle = configHeaderToValue.contains("logStyle") ? configHeaderToValue["logStyle"] : "";
    groupBoxPrefStyle = configHeaderToValue.contains("groupBoxPrefStyle") ? configHeaderToValue["groupBoxPrefStyle"] : "";
    resetButtonStyle = configHeaderToValue.contains("resetButtonStyle") ? configHeaderToValue["resetButtonStyle"] : "";
    filterButtonStyle = configHeaderToValue.contains("filterButtonStyle") ? configHeaderToValue["filterButtonStyle"] : "";
    goButtonStyle = configHeaderToValue.contains("goButtonStyle") ? configHeaderToValue["goButtonStyle"] : "";

    locationCBsStyle = configHeaderToValue.contains("locationCBsStyle") ? configHeaderToValue["locationCBsStyle"] : "";
    floorsNumbersWidgetStyle = configHeaderToValue.contains("floorsNumbersWidgetStyle") ? configHeaderToValue["floorsNumbersWidgetStyle"] : "";
    floorsNumbersWidgetHeader = configHeaderToValue.contains("floorsNumbersWidgetHeader") ? configHeaderToValue["floorsNumbersWidgetHeader"] : "";
    baseFloor = configHeaderToValue.contains("baseFloor") ? configHeaderToValue["baseFloor"] : "";
}

QString fontType;// = "font-family: \"Times New Roman\", Times, serif;";
QString fontSize;// = "font-size: 20px;";
QString border;// = "border: 1px solid black;";
QString globalTextAttributes;// = fontType + fontSize + border;
QString roomsXmlPath;// = "../Nav/dependencies/rooms.xml";

QString projectLogo;// = "../Nav/dependencies/LOGO.JPG";
QString projectTitleLabelText;// = "Navigation in Jacobs";
QString projectTitleLabelStyle;//  = "background-color: yellow;";
QString windowTitleText;//  = "Created By Adam Anan & Gilad Eini";
QString currentLocationLabelText;// = "I am here: ";

QString currentLocationLabelStyle;// = "background-color: green;";
QString destinationLabelText;// = "Take me to: ";
QString destinationLabelStyle;// = "background-color: red;";
QString logLabelText;// = "Directions";
QString logLabelStyle;// = "background-color: blue;";

QString logStyle;// = "background-color: grey;";
QString groupBoxPrefStyle;// = "";
QString resetButtonStyle;// = "";
QString filterButtonStyle;// = "";
QString goButtonStyle;// = "";

QString locationCBsStyle;// = "";
QString floorsNumbersWidgetStyle;// = "";
QString floorsNumbersWidgetHeader;// = "See only the following floors:";
QString baseFloor;// = "3"; //used in the filter. come in use when user uncheck all floors
};

class Nav : public QWidget
{
    Q_OBJECT

public:
    explicit Nav(QWidget *parent = 0);
    ~Nav();
    void readRoomsFromXml();
    QMap<QString,QString> readConfigXml(QString attName, QString attValue, QString xmlPath);
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
    configVars* config;
};


#endif // NAV_H
