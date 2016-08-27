#include "nav.h"



Nav::Nav(QWidget *parent):QWidget(parent)
{
    setObjectName("Nav gui manager");
    m_comboKey = comboKey;
    readAndCache();
    initOnce();
    init();
}

Nav::~Nav(){}

/*read files once at start up and cache them*/
void Nav::readAndCache()
{
    readRoomsFromXml();
}

void Nav::translateRoomsFromCppToQt()
{
    list<Node*> nodetList =  m_graph->GetGrapghNodes();
    for (Node* node : nodetList){
        m_nodesMap[node->GetId()] = node;
        QMap<QString,QString> room;
        room["Id"] = QString::number(node->GetId());
        room["Name"] = node->GetName().c_str();
        const pair<Direction, string> * neighbos = node->GetNeihbors();
        for (int i=0; i<NUM_OF_NEIGBHORS ; i++){
            room[dirMap2[neighbos[i].first]] = neighbos[i].second.c_str();
        }
        room["Floor"] = QString::number(node->GetNodeFloor());
        m_roomsObjects.push_back(room);
    }
}

void Nav::printRooms()
{
    QList<QString> headers({"Name" , "Id", "Floor", "North", "East", "South", "West"});
    QString strHeaders("");
    for(QString header : headers)
    {
        strHeaders.append(header + " ");
    }
    qDebug() << strHeaders;
    for (QMap<QString, QString> room : m_roomsObjects){
        QString strRoom("");
        strRoom.append(room[headers.at(0)]+ "    ");
        strRoom.append(room[headers.at(1)]+ "  ");
        strRoom.append(room[headers.at(2)]+ "     ");

        QString strNorth = room[headers.at(3)].isEmpty() ? "      " : room[headers.at(3)]+ "     ";
        strRoom.append(strNorth);
        QString strEast = room[headers.at(4)].isEmpty() ? "     " : room[headers.at(4)]+ "    ";
        strRoom.append(strEast);
        QString strSouth = room[headers.at(5)].isEmpty() ? "      " : room[headers.at(5)]+ "     ";
        strRoom.append(strSouth);
        QString strWest = room[headers.at(6)].isEmpty() ? "     " : room[headers.at(6)]+ "    ";
        strRoom.append(strWest);
        qDebug() << strRoom;
    }
}

void Nav::readRoomsFromXml()//temp solution
{
    //m_roomsObjects = readRoomsFromXmlUsingEngine(roomsXmlPath);
    bool succesReadingXml;
    m_graph = new Graph(roomsXmlPath.toUtf8().constData(), edgesXmlPath.toUtf8().constData(), succesReadingXml);
    if (!succesReadingXml){
        exitProgramWithErrMsg("Unable to read XML files.\nPlease check their paths in config.h are valid");
    }
    translateRoomsFromCppToQt();
    if(DEBUG) printRooms();
}

void Nav::initOnce()
{
    /*general attributes*/
    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
    /*widow title*/
    setWindowTitle(windowTitleText);
    /*project title*/
    QLabel* projectLabel = new QLabel(projectTitleLabelText);
    projectLabel->setStyleSheet(globalTextAttributes + projectTitleLabelStyle);
    projectLabel->setAlignment(Qt::AlignCenter);
    projectLabel->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Maximum);

    m_titleLayout = new QHBoxLayout();
    m_titleLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::MinimumExpanding,QSizePolicy::Maximum));
    m_titleLayout->addWidget(projectLabel);
    m_titleLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::MinimumExpanding,QSizePolicy::Maximum));

    /*control buttons*/
    QLabel* viewRoomsLabel = new QLabel(viewRoomsCbText);
    viewRoomsLabel->setStyleSheet(globalTextAttributes + viewRoomsCbStyle);
    m_viewRoomsCb = new QComboBox();
    m_viewRoomsCb->addItems(viewRoomsCb);
    m_viewRoomsCb->setCurrentText(m_comboKey);
    connect(m_viewRoomsCb, SIGNAL(activated(int)), this, SLOT(viewRoomsCbHasChangedSlot()));

    m_resetButton = new QPushButton("reset");
    connect(m_resetButton,SIGNAL(clicked(bool)),this,SLOT(resetSlot()));
    QVBoxLayout* controlButtonsVLayout = new QVBoxLayout();

    controlButtonsVLayout->addWidget(viewRoomsLabel);
    controlButtonsVLayout->addWidget(m_viewRoomsCb);
    controlButtonsVLayout->addWidget(m_resetButton);

    m_controlButtonsHLayout = new QHBoxLayout();
    m_controlButtonsHLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::MinimumExpanding,QSizePolicy::Maximum));
    m_controlButtonsHLayout->addLayout(controlButtonsVLayout);

    /*combos labels*/
    QLabel* currentLocationLabel = new QLabel(currentLocationLabelText);
    currentLocationLabel->setStyleSheet(globalTextAttributes + currentLocationLabelStyle);
    QLabel* destinationLabel = new QLabel(destinationLabelText);
    destinationLabel->setStyleSheet(globalTextAttributes + destinationLabelStyle);

    /*combos inside text*/
    m_currentLocationCb = new QComboBox();
    connect(m_currentLocationCb, SIGNAL(activated(int)), this, SLOT(currentLocationCbHasChangedSlot()));

    m_destinationCb = new QComboBox();
    connect(m_destinationCb, SIGNAL(activated(int)), this, SLOT(destinationCbHasChangedSlot()));

    /*go button*/
    m_goButton = new QPushButton("GO");
    connect(m_goButton,SIGNAL(clicked(bool)),this,SLOT(goWasPressedSlot()));

    /*combos layout*/
    QVBoxLayout* combosLabelsLayout = new QVBoxLayout;
    combosLabelsLayout->addWidget(currentLocationLabel);
    combosLabelsLayout->addWidget(destinationLabel);

    QVBoxLayout* cBLayout = new QVBoxLayout();
    cBLayout->addWidget(m_currentLocationCb);
    cBLayout->addWidget(m_destinationCb);

    m_cbAndLabelsLayout = new QHBoxLayout();
    m_cbAndLabelsLayout->addLayout(combosLabelsLayout);
    m_cbAndLabelsLayout->addLayout(cBLayout);
    m_cbAndLabelsLayout->addWidget(m_goButton);
    m_cbAndLabelsLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::MinimumExpanding,QSizePolicy::Maximum));

    /*text box for direction and video player*/

    QLabel* logLabel = new QLabel(logLabelText);
    logLabel->setStyleSheet(globalTextAttributes + logLabelStyle);
    logLabel->setAlignment(Qt::AlignCenter);

    m_log = new QTextEdit();
    m_log->setStyleSheet(globalTextAttributes + logStyle);
    m_log->setReadOnly(true);

    m_mediaPlayer = new QMediaPlayer;
    m_videoWidget = new QVideoWidget;

    QVBoxLayout* logLayout = new QVBoxLayout();
    logLayout->addWidget(logLabel);
    logLayout->addWidget(m_log);


    m_logSpacerLayout = new QHBoxLayout();
    m_logSpacerLayout->addWidget(m_videoWidget);
    m_logSpacerLayout->addLayout(logLayout);
    m_logSpacerLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::MinimumExpanding,QSizePolicy::Maximum));
}

void Nav::init()
{
    /*general attributes*/
    m_mainVLayout = new QVBoxLayout;
    m_mainVLayout->setObjectName("main vertical layout");
    setLayout(m_mainVLayout);

    m_mainVLayout->addLayout(m_titleLayout);
    m_mainVLayout->addLayout(m_controlButtonsHLayout);

    /*combos inside text*/
    QStringList tags = getRoomsTagsToPlaceInComboBox();
    m_currentLocationCb->addItems(tags);
    m_destinationCb->addItems(tags);
    currentLocationCbHasChangedSlot();
    destinationCbHasChangedSlot();

    m_mainVLayout->addLayout(m_cbAndLabelsLayout);
    m_mainVLayout->addLayout(m_logSpacerLayout);




    m_mediaPlayer->setVideoOutput(m_videoWidget); //where to stream the video
    m_videoWidget->show();

    m_mediaPlayer->setMedia(QUrl::fromLocalFile(videoTest)); //video location
    m_mediaPlayer->setPosition(2000); // starting index time

    m_mediaPlayer->play();
}

void Nav::resetSlot()
{
    m_log->clear();
    m_currentLocationCb->clear();
    m_destinationCb->clear();
    m_mainVLayout->removeItem(m_titleLayout);
    m_mainVLayout->removeItem(m_controlButtonsHLayout);
    m_mainVLayout->removeItem(m_cbAndLabelsLayout);
    m_mainVLayout->removeItem(m_logSpacerLayout);
    delete m_mainVLayout;
    init();
}

Node *Nav::findNodeByStr(QString str)
{
    int id = -1;
    for(QMap<QString,QString> room : m_roomsObjects)
    {
        if(str == room[m_comboKey]){
            id = room["Id"].toInt();
        }
    }
    return m_nodesMap[id];
}

void Nav::currentLocationCbHasChangedSlot(){
    m_currentRoom = findNodeByStr(m_currentLocationCb->currentText());
}

void Nav::destinationCbHasChangedSlot(){
    m_destRoom = findNodeByStr(m_destinationCb->currentText());
}

void Nav::goWasPressedSlot()
{
    m_log->clear();
    QString currentRoom(m_currentLocationCb->currentText());
    QString destRoom(m_destinationCb->currentText());
    m_log->append( "route from " + currentRoom + " to " + destRoom + ": " );
    if (currentRoom == destRoom )
        m_log->append( "Stay where you are :)" );
    else{
        QList<pathRoomQt> shortestPathQt = translateShortestPathFromCppToQt();
        appendShortestPathToLog(shortestPathQt);
    }


}

QList<pathRoomQt> Nav::translateShortestPathFromCppToQt(){
    list<pathRoom> shortestPath = m_graph->GetShortestpath(m_currentRoom, m_destRoom);
    QList<pathRoomQt> shortestPathQt;
    for(pathRoom room : shortestPath){
        pathRoomQt roomQt;
        roomQt.room = room.room;
        roomQt.direction = room.direction.c_str();
        roomQt.distance = room.distance;
        roomQt.nextRoomInPathId = room.nextRoomInPathId;
        shortestPathQt.push_back(roomQt);
    }
    if(DEBUG) printShortestPath(shortestPathQt);
    return shortestPathQt;
}

void Nav::printShortestPath(QList<pathRoomQt> shortestPathQt)
{
    for (int i=0; i<shortestPathQt.size(); ++i )
    {
        QString strRoomInfo("");
        strRoomInfo.append(shortestPathQt[i].room->GetName().c_str());
        strRoomInfo.append("; ");
        strRoomInfo.append(shortestPathQt[i].direction);
        strRoomInfo.append("; ");
        strRoomInfo.append(QString::number(shortestPathQt[i].distance));
        strRoomInfo.append("; ");
        strRoomInfo.append(QString::number(shortestPathQt[i].nextRoomInPathId));
        qDebug() << strRoomInfo;
    }
}

void Nav::appendShortestPathToLog(QList<pathRoomQt> shortestPathQt)
{
    m_log->append("Start from " + getRoomValueByIdAndComboKey(shortestPathQt[0].room->GetId()));
    for(pathRoomQt roomInPath: shortestPathQt ){
        m_log->append("Walk " + QString::number(roomInPath.distance) + " meters " +
                      roomInPath.direction +
                      " to " + getRoomValueByIdAndComboKey(roomInPath.nextRoomInPathId) + "." );
    }
}

QString Nav::getRoomValueByIdAndComboKey(int id)
{
    for(QMap<QString,QString> room : m_roomsObjects){ if (room["Id"] == QString::number(id)) return room[m_comboKey]; }
    return NULL;
}

void Nav::viewRoomsCbHasChangedSlot()
{
    m_comboKey = m_viewRoomsCb->currentText();
    resetSlot();
}

QStringList Nav::getRoomsTagsToPlaceInComboBox()
{
    QStringList tags;
    for(QMap<QString,QString> room : m_roomsObjects){
        tags.push_back(room[m_comboKey]);
    }
    return tags;
}

void Nav::exitProgramWithErrMsg(QString errMsg)
{
    showQmsgBox(errMsg);
    exit(EXIT_FAILURE);
}

void Nav::showQmsgBox(QString msg)
{
    QMessageBox err;
    err.setText(msg);
    err.exec();
}
