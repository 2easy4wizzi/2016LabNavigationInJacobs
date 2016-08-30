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
        room["Number"] = (node->GetNumber().c_str());
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


    m_resetButton = new QPushButton("reset");
    connect(m_resetButton,SIGNAL(clicked(bool)),this,SLOT(resetSlot()));


    m_controlButtonsHLayout = new QHBoxLayout();
    m_controlButtonsHLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::MinimumExpanding,QSizePolicy::Maximum));
    m_controlButtonsHLayout->addWidget(m_resetButton);

    /*group box view room text by - name vs number*/
    QGroupBox *groupBoxViewBy = new QGroupBox("View rooms by:");
    groupBoxViewBy->setStyleSheet(groupBoxViewByStyle);
    m_groupBoxViewByRadioName = new QRadioButton("Name");
    connect(m_groupBoxViewByRadioName, SIGNAL(clicked(bool)), this, SLOT(viewRoomsByHasChangedSlot()));

    m_groupBoxViewByRadioNumber = new QRadioButton("Number");
    connect(m_groupBoxViewByRadioNumber, SIGNAL(clicked(bool)), this, SLOT(viewRoomsByHasChangedSlot()));

    m_groupBoxViewByRadioName->setChecked(true);
    QVBoxLayout *vboxViewBy = new QVBoxLayout;
    vboxViewBy->addWidget(m_groupBoxViewByRadioName);
    vboxViewBy->addWidget(m_groupBoxViewByRadioNumber);
    vboxViewBy->addStretch(1);
    groupBoxViewBy->setLayout(vboxViewBy);
    m_controlButtonsHLayout->addWidget(groupBoxViewBy);

    /*group box preference - elevator vs stairs*/
    QGroupBox *groupBoxPref = new QGroupBox("Take me from the");
    groupBoxPref->setStyleSheet(groupBoxPrefStyle);
    m_groupBoxPrefRadioDefault = new QRadioButton("Fastest way");
    m_groupBoxPrefRadioStiars = new QRadioButton("Stiars");
    m_groupBoxPrefRadioElevator = new QRadioButton("Elevator");

    connect(m_groupBoxPrefRadioDefault, SIGNAL(clicked(bool)), this, SLOT(prefWasChangedSlot()));
    connect(m_groupBoxPrefRadioStiars, SIGNAL(clicked(bool)), this, SLOT(prefWasChangedSlot()));
    connect(m_groupBoxPrefRadioElevator, SIGNAL(clicked(bool)), this, SLOT(prefWasChangedSlot()));
    prefWasChangedSlot();

    m_groupBoxPrefRadioDefault->setChecked(true);
    QVBoxLayout *vboxPref = new QVBoxLayout;
    vboxPref->addWidget(m_groupBoxPrefRadioDefault);
    vboxPref->addWidget(m_groupBoxPrefRadioStiars);
    vboxPref->addWidget(m_groupBoxPrefRadioElevator);
    vboxPref->addStretch(1);
    groupBoxPref->setLayout(vboxPref);





    m_controlButtonsHLayout->addWidget(groupBoxPref);

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




    QVBoxLayout* logLayout = new QVBoxLayout();
    logLayout->addWidget(logLabel);
    logLayout->addWidget(m_log);

    m_mediaPlayer = new QMediaPlayer;
    m_videoWidget = new QVideoWidget;



    m_logSpacerLayout = new QHBoxLayout();
    m_logSpacerLayout->addWidget(m_videoWidget);
    m_logSpacerLayout->addLayout(logLayout);
    //m_logSpacerLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::MinimumExpanding,QSizePolicy::Maximum));



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

    playVideoFromTo(videoTest,7000,8000);

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
    list<pathRoom> shortestPath = m_graph->GetShortestpath(m_currentRoom, m_destRoom, m_pref);
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
    m_log->append("\nStart from " + getRoomFieldById(shortestPathQt[0].room->GetId(),m_comboKey) + " :");
    int i = 0;
    for(pathRoomQt roomInPath: shortestPathQt ){

        QString roomName = roomInPath.room->GetName().c_str();
        int nextRoomInPathId = roomInPath.nextRoomInPathId;

        if(roomName.contains("Elevator") && getRoomFieldById(nextRoomInPathId, "Name").contains("Elevator")){
            int currentFloor = roomInPath.room->GetNodeFloor();
            int desFloor = getRoomFieldById(nextRoomInPathId,"Floor").toInt();
            QString text = currentFloor > desFloor ?
                        (QString::number(++i) + ") Use the elevator to go down to floor" + QString::number(desFloor) + ".") :
                        (QString::number(++i) + ") Use the elevator to go up to floor"   + QString::number(desFloor) + ".");
            m_log->append(text);
        }
        else if(roomName.contains("Stairs")&& getRoomFieldById(nextRoomInPathId, "Name").contains("Stairs")){
            int currentFloor = roomInPath.room->GetNodeFloor();
            int desFloor = getRoomFieldById(nextRoomInPathId,"Floor").toInt();
            QString text = currentFloor > desFloor ?
                        (QString::number(++i) + ") Go down the stairs to floor" + QString::number(desFloor) + ".") :
                        (QString::number(++i) + ") Go up the stairs to floor"   + QString::number(desFloor) + ".");
            m_log->append(text);
        }
        else{
            m_log->append(QString::number(++i) + ") Walk " + QString::number(roomInPath.distance) +
                          " meters " + roomInPath.direction +
                          " to " + getRoomFieldById(nextRoomInPathId,m_comboKey) + "." );
        }





    }
}

void Nav::playVideoFromTo(QString videoPath, int from, int to)
{
    m_videoWidget->show();
    m_mediaPlayer->setVideoOutput(m_videoWidget); //where to stream the video
    m_mediaPlayer->setMedia(QUrl::fromLocalFile(videoPath)); //video location
    m_mediaPlayer->setPosition(from); // starting index time
    m_mediaPlayer->play();
}

QString Nav::getRoomFieldById(int id, QString field)
{
    for(QMap<QString,QString> room : m_roomsObjects){ if (room["Id"] == QString::number(id)) return room[field]; }
    return NULL;
}


void Nav::viewRoomsByHasChangedSlot()
{
    m_comboKey = m_groupBoxViewByRadioName->isChecked() ? m_groupBoxViewByRadioName->text() : m_groupBoxViewByRadioNumber->text();
    resetSlot();
}

void Nav::prefWasChangedSlot()
{
    if(m_groupBoxPrefRadioDefault->isChecked())
    {
        m_pref = EdgeType::Regular;
    }
    else if(m_groupBoxPrefRadioStiars->isChecked())
    {
        m_pref = EdgeType::Stairs;
    }
    else if(m_groupBoxPrefRadioElevator->isChecked())
    {
        m_pref = EdgeType::Elevator;
    }
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
