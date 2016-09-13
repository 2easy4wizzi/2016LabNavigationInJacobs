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
    //todo - change all config fields to Private and use getters instead config->field.
    //to make the fields "const"
    configVars(QMap<QString,QString> configHeaderToValue);

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
    void appendShortestPathToLog(QString movieMessege, QString color = "black");
    void playVideoFromTo(bool appendedThisMoveToLogBefore);
    void findWitchButtonIsOnPref();
    void testingFuncton();
    void stopDeleteAndMakeNewMediaPlayer(bool firstTime);

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
    void rePlaySlot();
    void nextSlot();
    void closeGroupBoxdestinationCbWidgetSlot();
    void closeGroupBoxCurrentLocationCbWidgetSlot();
    void stateOfMediaPlayerChangedSlot(QMediaPlayer::State state = QMediaPlayer::StoppedState);
    void qTimerTimeoutSlot();

signals:


private:
    void exitProgramWithErrMsg(QString errMsg);
    void showQmsgBox(QString msg);
    QList<QMap<QString,QString>> m_roomsObjects;//each room object got a list of attributes
    QVBoxLayout* m_mainVLayout;
    QComboBox * m_currentLocationCb;
    QGroupBox *m_groupBoxCurrentLocationCbWidget;
    QComboBox * m_destinationCb;
    QGroupBox *m_groupBoxdestinationCbWidget;
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
    QRadioButton *m_groupBoxPrefRadioDefault;
    QRadioButton *m_groupBoxPrefRadioStairs;
    QRadioButton *m_groupBoxPrefRadioElevator;
    QRadioButton *m_groupBoxViewByRadioName;
    EdgeType m_pref;
    QMap <int,QString> m_floorToShow1;
    QMap <int,QString> m_floorToShow2;
    Node* m_roomVideoDisplay;
    QList<Node*> m_shortestPathQt;
    int m_videoPlayerCounter;
    configVars* config;
    bool m_playWithOutPause;
    QCheckBox* m_playWithOutPauseCheckBox;
    QTimer* m_qTimer;
    bool m_moreVideoToShowOnThisNode;
    QPushButton* m_rePlay;
    QPushButton* m_next;
};


#endif // NAV_H
