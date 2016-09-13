#include "nav.h"

Nav::Nav(QWidget *parent):QWidget(parent)
{
    setObjectName("Nav gui manager");
    readRoomsFromXml();
    initOnce();
    init();
}

void Nav::readRoomsFromXml()
{
    config = new configVars( readConfigXml("Name", "Value", "../Nav/dependencies/config.xml") );
    bool succesReadingXml;
    m_graph = new Graph(config->roomsXmlPath.toUtf8().constData(), succesReadingXml);
    if (!succesReadingXml){
        exitProgramWithErrMsg("Unable to read XML files.\nPlease check that their paths in config.xml are valid");
    }
    translateRoomsFromCppToQt();
}

QMap<QString, QString> Nav::readConfigXml(QString attName, QString attValue, QString xmlPath)//read properties of gui styleing and text
{
    QDomDocument xmlBOM;
    QFile f(xmlPath);
    if (!f.open(QIODevice::ReadOnly )){
        exitProgramWithErrMsg("Cant find ../Nav/dependencies/config.xml");
    }
    xmlBOM.setContent(&f);
    f.close();
    QDomElement root = xmlBOM.documentElement();
    QDomElement child = root.firstChild().toElement();
    QMap<QString, QString> retMap;
    // Loop while there is a child
    while(!child.isNull())
    {
        if (child.tagName()=="Entry")
        {
            QString name = child.attribute(attName,"No name");
            QString value = child.attribute(attValue,"No value");
            value.replace("\\'","\"");
            retMap[name] = value;

        }
        child = child.nextSibling().toElement();
    }
    return retMap;
}

//creating 3 containers.
//1. m_roomsObjects - mapping each field name(QString) to his value(QString)
//2. m_nodesMap     - mapping each id(int) to his Node object(Node*)
void Nav::translateRoomsFromCppToQt()
{
    list<Node*> nodetList =  m_graph->GetGrapghNodes();
    for (Node* node : nodetList){
        m_nodesMap[node->GetId()] = node;
        QMap<QString,QString> room;
        room[fieldID] = QString::number(node->GetId());
        room[fieldName] = node->GetName().c_str();
        const int * classes = node->GetClasses();
        QMap<int,QString> helper = {{0,"A"} , {1,"B"}, {2,"C"}, {3,"D"}, {4,"E"} };
        for (int i=0; i<NUMBER_OF_CLASSES ; i++){
            QString className("Class" + helper[i]);
            room[className] = QString::number(classes[i]);
        }
        room[fieldFloor] = QString::number(node->GetNodeFloor());
        room[fieldNumber] = (node->GetNumber().c_str());
        m_roomsObjects.push_back(room);
    }
}


//could happen more than once(for example: if you press 'reset' button)
void Nav::init()
{
    stopDeleteAndMakeNewMediaPlayer(true);
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
}

//start over
void Nav::resetSlot()
{
    m_log->clear();
    m_currentLocationCb->clear();
    m_destinationCb->clear();
    m_mainVLayout->removeItem(m_titleLayout);
    m_mainVLayout->removeItem(m_controlButtonsHLayout);
    m_mainVLayout->removeItem(m_cbAndLabelsLayout);
    m_mainVLayout->removeItem(m_logSpacerLayout);
    m_roomVideoDisplay = NULL;
    m_shortestPathQt.clear();
    QColor qcolor("black");
    m_log->setTextColor(qcolor);
    disconnect(m_mediaPlayer,  SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(stateOfMediaPlayerChangedSlot(QMediaPlayer::State)) );
    delete m_mainVLayout;
    init();
}

//given an attribute(QString), go over m_roomsObjects and look for it. return the Node with the name==str.
Node *Nav::findNodeByStr(QString str)
{
    int id = -1;
    for(QMap<QString,QString> room : m_roomsObjects)
    {
        if(str == room[fieldName] || str == (room[fieldName] + "(" +room[fieldNumber] + ")")){
            id = room[fieldID].toInt();
        }
    }
    return m_nodesMap[id];
}

//the next 2 slots: always update the current and destanation rooms in members
void Nav::currentLocationCbHasChangedSlot(){
    m_currentRoom = findNodeByStr(m_currentLocationCb->currentText());
}

void Nav::destinationCbHasChangedSlot(){
    m_destRoom = findNodeByStr(m_destinationCb->currentText());
}

void Nav::goWasPressedSlot()
{
    m_roomVideoDisplay = NULL;
    m_shortestPathQt.clear();
    m_log->clear();
    QColor qcolor("black");
    m_log->setTextColor(qcolor);

    QString currentRoom(m_currentLocationCb->currentText());
    QString destRoom(m_destinationCb->currentText());
    m_log->append( "Route from " + currentRoom + " to " + destRoom + ": " );
    if (currentRoom == destRoom )
        m_log->append( "Stay where you are :)" );
    else{
        QString currentRoom(m_currentLocationCb->currentText());
        m_log->append("\nStart from " + currentRoom + " :");
        translateShortestPathFromCppToQt(); //ask graph class to find shortest path and convert list<Node*> to QT::QList.
        m_roomVideoDisplay = m_shortestPathQt.at(0);//set m_roomVideoDisplay to first video assuming the graph is well connected(otherwise it might be null)
        m_videoPlayerCounter = 0;
        m_playWithOutPause = m_playWithOutPauseCheckBox->isChecked();
        //disable buttons if user asked for no pauses
        m_next->setDisabled(m_playWithOutPause);
        m_rePlay->setDisabled(m_playWithOutPause);
        playVideoFromTo(false);//plays a video's part(could be 2 seconds from 10 seconds video) given a Node
    }
}

void Nav::translateShortestPathFromCppToQt(){//get the shortest pass from s to t (and with preference) and copy it to QT object
    list<Node*> shortestPath = m_graph->GetShortestpath(m_currentRoom, m_destRoom, m_pref);//graph::GetShortestpath
    for(Node* room : shortestPath){
        m_shortestPathQt.push_back(room);
    }
    shortestPath.clear();
}


//appending instructions to the Log.
void Nav::appendShortestPathToLog(QString movieMessege, QString color) // 3 cases. elevator, stairs and regular. print by case.
{
    int i = 0;
    for (Node*room : m_shortestPathQt)
    {
        if(room == m_roomVideoDisplay)
        {
            break;
        }
        ++i;
    }
    QString textToAppend;
    QColor qcolor(color);
    m_log->setTextColor(qcolor);
    Node* roomInPath = m_roomVideoDisplay;

    QString roomName = roomInPath->GetName().c_str();
    int nextRoomInPathId = roomInPath->nextRoomInPathId();

    if(roomName.contains(fieldElevator) && getRoomFieldById(nextRoomInPathId, fieldName).contains(fieldElevator)){
        int currentFloor = roomInPath->GetNodeFloor();
        int desFloor = getRoomFieldById(nextRoomInPathId,fieldFloor).toInt();
        textToAppend = currentFloor > desFloor ?
                    (QString::number(++i) + ") Use the elevator to go down to floor " + QString::number(desFloor) + ".") :
                    (QString::number(++i) + ") Use the elevator to go up to floor "   + QString::number(desFloor) + ".");
        textToAppend.append(movieMessege);
        m_log->append(textToAppend);
    }
    else if(roomName.contains(fieldStairs)&& getRoomFieldById(nextRoomInPathId, fieldName).contains(fieldStairs) && roomInPath->GetNodeFloor() != getRoomFieldById(nextRoomInPathId, fieldFloor).toInt()){
        int currentFloor = roomInPath->GetNodeFloor();
        int desFloor = getRoomFieldById(nextRoomInPathId,fieldFloor).toInt();
        textToAppend = currentFloor > desFloor ?
                    (QString::number(++i) + ") Go down the stairs to floor " + QString::number(desFloor) + ".") :
                    (QString::number(++i) + ") Go up the stairs to floor "   + QString::number(desFloor) + ".");
        textToAppend.append(movieMessege);
        m_log->append(textToAppend);
    }
    else{
        QString name = getRoomFieldById(roomInPath->nextRoomInPathId(), fieldName);
        QString number = getRoomFieldById(roomInPath->nextRoomInPathId(), fieldNumber);

        QString tag = name;
        if (number != "0" && number != "-1" && !name.contains(number)) tag.append("(" + number + ")");
        textToAppend = QString::number(++i) + ") Walk " + QString::number(roomInPath->distanceToNextNodeInPath()) +
                " meters to " + tag + "." ;
        textToAppend.append(movieMessege);
        m_log->append(textToAppend);
    }
}


void Nav::playVideoFromTo(bool appendedThisMoveToLogBefore)//play Node's video part
{
    /*for this node in the path: look in his list of movies how many could be combined on this move*/
    videoInfo* videoInfoOfNodesInPath =  m_roomVideoDisplay->GetAllVideoInfos();//get all videos in current step
    videoInfo currentVideoInf = videoInfoOfNodesInPath[m_videoPlayerCounter];//start with just 1
    int startIndex = currentVideoInf._startIndex;
    int endIndex = currentVideoInf._endIndex;
    QString videoPath = currentVideoInf._pathToVideo.c_str();

    int tempCounter = m_videoPlayerCounter + 1 ;//try to combine videos. if we have route 1->2 and 2->3 and they share the same video, play it like its one video 1->3


    if(!appendedThisMoveToLogBefore) //if pressed replay, dont append to log again
    {
        QString movieMessege = (videoPath.isEmpty()) ? (" (Need to insert movie)") : "";
        appendShortestPathToLog(movieMessege, "red");
    }

    m_rePlay->setDisabled(videoPath.isEmpty());
    if(!videoPath.isEmpty()){
        while (tempCounter+1 <= m_roomVideoDisplay->videoInfoOfNodesInPathConter() && videoInfoOfNodesInPath[tempCounter]._pathToVideo == currentVideoInf._pathToVideo )
        {
            m_videoPlayerCounter = tempCounter;
            endIndex = videoInfoOfNodesInPath[tempCounter]._endIndex;//keep "pushing" the end index if video is the same
            tempCounter++;
        }
        m_mediaPlayer->blockSignals(true); //changing media cause unwanted signals to fire
        m_mediaPlayer->setMedia(QUrl::fromLocalFile(videoPath)); //video location
        m_mediaPlayer->setPosition(startIndex); // starting index time
        m_videoWidget->show();
        m_mediaPlayer->play();
        m_mediaPlayer->blockSignals(false);
        if(endIndex != -1)//if != -1 than the movie needs to be pause at endindex - startIndex
        {
            m_qTimer->stop();
            m_qTimer->setInterval(endIndex-startIndex);
            m_qTimer->setSingleShot(true);
            m_qTimer->start();
        }
    }
    else{//no movie
        if (m_playWithOutPause)//wait 1.5 secs for the user to read and continue
            QTimer::singleShot(1500,this, SLOT(stateOfMediaPlayerChangedSlot()));
        else
            stopDeleteAndMakeNewMediaPlayer(false); //no video - show default logo
    }

    m_videoPlayerCounter++;//advance the movie index of this current node.
    //let the program know this node didnt finish all his movies. he played just 1 part(possibly combined)
    m_moreVideoToShowOnThisNode = (m_videoPlayerCounter >= m_roomVideoDisplay->videoInfoOfNodesInPathConter()) ? false : true ;
}

//testing function. remove the comment in main.cpp and change the wanted actions. will help saving time debugging
void Nav::testingFuncton()
{
    bool testingmode;
    testingmode = false;
    testingmode = true;
    if(!testingmode) return;
    QMap<int, QString> floorsToShow;
    floorsToShow.insert(4,"4");
    floorsToShow.insert(3,"3");
    QStringList tagsC = getRoomsTagsToPlaceInComboBox(floorsToShow);
    QStringList tagsD = getRoomsTagsToPlaceInComboBox(floorsToShow);

    //cout << tagsC << tagsC.size();

    tagsC.clear();
    tagsC += QString("Jackbobs entrance");
//    tagsC += QString("CS private room(418)");
//    tagsC += QString("Class(303)");
    //tagsC += QString("Copy room(404)");
    //cout << tagsC;
    tagsD.clear();
//    tagsD += QString("Dan Feldman(413)");
    //    tagsD += QString("Or Donkelman(408)");
    tagsD += QString("Bathroom floor 3");
//    tagsD += QString("Office(301)");
    //cout << tagsD;
//    int iterNumber = 60000;
//    int itersOuter = 1;
//    int itersInner = 1;
    int i = 0;
    for(QString strC : tagsC)
    {
        for(QString strD : tagsD)
        {
            m_currentLocationCb->setCurrentText(strC);
            m_destinationCb->setCurrentText(strD);

            m_currentRoom = findNodeByStr(m_currentLocationCb->currentText());
            m_destRoom = findNodeByStr(m_destinationCb->currentText());
//            m_playWithOutPauseCheckBox->setChecked(true);
            goWasPressedSlot();
//            for(int i = 0; i < m_shortestPathQt.size(); ++i)
//            {
//                nextSlot();
//            }
//            showQmsgBox(strC + " to " + strD);
            i++;
        }
    }
}

void Nav::stopDeleteAndMakeNewMediaPlayer(bool firstTime)
{
    if(!firstTime){
        m_mediaPlayer->stop();
        delete m_mediaPlayer;
    }
    m_mediaPlayer = new QMediaPlayer;
    connect(m_mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(stateOfMediaPlayerChangedSlot(QMediaPlayer::State)) );
    m_mediaPlayer->setAudioRole(QAudio::VideoRole);
    m_mediaPlayer->setVideoOutput(m_videoWidget); //where to stream the video
}

QString Nav::getRoomFieldById(int id, QString field)//given an Id, retrun the wanted value.
{
    for(QMap<QString,QString> room : m_roomsObjects){ if (room[fieldID] == QString::number(id)) return room[field]; }
    return NULL;
}

//save in a member the user preference(regular, stairs, elavator)
void Nav::prefWasChangedSlot()
{
    if(m_groupBoxPrefRadioDefault->isChecked())
    {
        m_pref = EdgeType::Regular;
    }
    else if(m_groupBoxPrefRadioStairs->isChecked())
    {
        m_pref = EdgeType::Stairs;
    }
    else if(m_groupBoxPrefRadioElevator->isChecked())
    {
        m_pref = EdgeType::Elevator;
    }
}

//show current filter widget
void Nav::showFilter1Slot()
{
    m_groupBoxCurrentLocationCbWidget->show();
}

//show destanation filter widget
void Nav::showFilter2Slot()
{
    m_groupBoxdestinationCbWidget->show();
}

//rebuild current combo box rooms
void Nav::updateFilter1Slot()
{
    m_floorToShow1.clear();
    QList<QCheckBox*> checkBoxes = m_groupBoxCurrentLocationCbWidget->findChildren<QCheckBox*>("filter1");
    for(QCheckBox* checkbox : checkBoxes){
        if(checkbox->isChecked()){
            m_floorToShow1.insert(checkbox->text().toInt(), checkbox->text());
        }
    }
    if(m_floorToShow1.size() == 0)
    {
        for(QCheckBox* checkbox : checkBoxes){
            if(checkbox->text() == config->baseFloor){
                checkbox->setChecked(true);
                m_floorToShow1.insert(checkbox->text().toInt(), checkbox->text());
            }
        }
    }
    QStringList tags = getRoomsTagsToPlaceInComboBox(m_floorToShow1);
    m_currentLocationCb->clear();
    m_currentLocationCb->addItems(tags);
    currentLocationCbHasChangedSlot();
}

//rebuild destanation combo box rooms
void Nav::updateFilter2Slot()
{
    m_floorToShow2.clear();
    QList<QCheckBox*> checkBoxes = m_groupBoxdestinationCbWidget->findChildren<QCheckBox*>("filter2");
    for(QCheckBox* checkbox : checkBoxes){
        if(checkbox->isChecked()){
            m_floorToShow2.insert(checkbox->text().toInt(), checkbox->text());
        }
    }
    if(m_floorToShow2.size() == 0)
    {
        for(QCheckBox* checkbox : checkBoxes){
            if(checkbox->text() == config->baseFloor){
                checkbox->setChecked(true);
                m_floorToShow2.insert(checkbox->text().toInt(), checkbox->text());
            }
        }
    }
    QStringList tags = getRoomsTagsToPlaceInComboBox(m_floorToShow2);
    m_destinationCb->clear();
    m_destinationCb->addItems(tags);
    destinationCbHasChangedSlot();
}

//replay was pressed
void Nav::rePlaySlot()
{
    m_qTimer->stop();
    if(m_roomVideoDisplay)
    {
        //todo - save previous node start on a member
        videoInfo* videoInfoOfNodesInPath =  m_roomVideoDisplay->GetAllVideoInfos();//get all videos in current step
        videoInfo currentVideoInf = videoInfoOfNodesInPath[m_videoPlayerCounter - 1];//get current
        QString videoPath = currentVideoInf._pathToVideo.c_str();//save video path
        if(!videoPath.isEmpty())
        {
            m_videoPlayerCounter--;
            int tempCounter = m_videoPlayerCounter - 1;
            //#mark
            while (tempCounter-1 >= 0 && videoInfoOfNodesInPath[tempCounter]._pathToVideo == currentVideoInf._pathToVideo )//reverting counter back to the begining (not necessarily 0)
            {
                m_videoPlayerCounter = tempCounter;
                tempCounter--;
            }
            playVideoFromTo(true);
        }
    }
}

void Nav::nextSlot()
{
    if(m_roomVideoDisplay)
    {
        m_qTimer->stop();
        /*clear last Red Line in Log*/
        QTextCursor cursor = m_log->textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.select(QTextCursor::LineUnderCursor);
        cursor.removeSelectedText();
        cursor.deletePreviousChar(); // Added to trim the newline char when removing last line
        m_log->setTextCursor(cursor);

        appendShortestPathToLog("");//append Black Line

        //look for next room. todo - keep index as member
        Node* tmpRoom = NULL;
        for (int i=0; i < m_shortestPathQt.size(); ++i)
        {
            if(m_shortestPathQt.at(i) == m_roomVideoDisplay && (i<m_shortestPathQt.size()-1) )
            {
                tmpRoom = m_shortestPathQt.at(i + 1);
                break;
            }
        }
        if(tmpRoom)//if room exist, continue
        {
            m_roomVideoDisplay = tmpRoom;
            m_videoPlayerCounter = 0;
            playVideoFromTo(false);
        }
        else{
            m_log->append("\nThat was the last step.\nClick on Go to repeat the instructions");
            m_next->setDisabled(true);
            m_rePlay->setDisabled(true);
            stopDeleteAndMakeNewMediaPlayer(false);
        }
    }
}

void Nav::closeGroupBoxdestinationCbWidgetSlot()
{
    m_groupBoxdestinationCbWidget->close();
}

void Nav::closeGroupBoxCurrentLocationCbWidgetSlot()
{
    m_groupBoxCurrentLocationCbWidget->close();
}

void Nav::stateOfMediaPlayerChangedSlot(QMediaPlayer::State state)
{
    if(state == QMediaPlayer::StoppedState || state == QMediaPlayer::PausedState){
        if(m_moreVideoToShowOnThisNode){
            playVideoFromTo(true);
        }
        else if(m_playWithOutPause){
            nextSlot();
        }
    }
}

void Nav::qTimerTimeoutSlot()
{
    m_mediaPlayer->pause();
}

//function that build the combo boxes labels
QStringList Nav::getRoomsTagsToPlaceInComboBox(QMap<int, QString> floorsToShow)
{
    QStringList tags;
    QMap <int,QString> sortKeyAndTags;
    int i = 0;
    for(QMap<QString,QString> room : m_roomsObjects){
        if(floorsToShow.isEmpty() || floorsToShow.values().contains(room[fieldFloor]))
        {
            if(room[fieldNumber] == "0"){ //rooms like bathroom with no number
                sortKeyAndTags.insert(room[fieldFloor].toInt()*1000-(++i), room[fieldName]);
            }
            else if(room[fieldNumber] == "-1"){ //rooms that help on the route but are not a destanation
                //do nothing
            }
            else{
                QString tag = room[fieldName].contains(room[fieldNumber]) ? room[fieldName] : room[fieldName] + "(" + room[fieldNumber] + ")";
                int tmpNum = room[fieldNumber].toInt();
                int number = tmpNum  > 999 ? tmpNum : tmpNum*10; //check. if>999 -> number already normalized(for sorting)
                sortKeyAndTags.insert(number, tag);
            }
        }
    }
    for(QMap<int,QString>::iterator it = sortKeyAndTags.begin(); it!=sortKeyAndTags.end(); ++it){
        tags.push_back(it.value());
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
    err.move(300,300);
    err.setText(msg);
    err.exec();
}

configVars::configVars(QMap<QString, QString> configHeaderToValue)
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

//happens once
void Nav::initOnce()
{
    m_roomVideoDisplay = NULL;
    m_shortestPathQt.clear();
    m_qTimer = new QTimer();
    connect(m_qTimer, SIGNAL(timeout()), SLOT(qTimerTimeoutSlot()));
    /*general attributes*/
    this->setFixedSize(QSize(1280,720));

    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
    /*widow title*/
    setWindowTitle(config->windowTitleText);
    /*project title*/
    QLabel* projectLabel = new QLabel(config->projectTitleLabelText);
    projectLabel->setStyleSheet(config->globalTextAttributes + config->projectTitleLabelStyle);
    projectLabel->setAlignment(Qt::AlignCenter);
    projectLabel->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Maximum);

    m_titleLayout = new QHBoxLayout();
    m_titleLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::MinimumExpanding,QSizePolicy::Maximum));
    m_titleLayout->addWidget(projectLabel);
    m_titleLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::MinimumExpanding,QSizePolicy::Maximum));

    /*control buttons*/


    QPushButton* m_resetButton = new QPushButton("reset");
    m_resetButton->setStyleSheet(config->resetButtonStyle);
    connect(m_resetButton,SIGNAL(clicked(bool)),this,SLOT(resetSlot()));

    m_playWithOutPauseCheckBox = new QCheckBox("play without pause");
    //m_playWithOutPauseCheckBox->setStyleSheet(config->resetButtonStyle);


    QVBoxLayout *controlButtonsVLayout = new QVBoxLayout;
    controlButtonsVLayout->addWidget(m_resetButton);
    controlButtonsVLayout->addWidget(m_playWithOutPauseCheckBox);

    m_controlButtonsHLayout = new QHBoxLayout();
    m_controlButtonsHLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::MinimumExpanding,QSizePolicy::Maximum));
    m_controlButtonsHLayout->addLayout(controlButtonsVLayout);


    /*group box preference - elevator vs stairs*/
    QGroupBox *groupBoxPref = new QGroupBox("Take me from the");
    groupBoxPref->setStyleSheet(config->groupBoxPrefStyle);
    m_groupBoxPrefRadioDefault = new QRadioButton("Fastest way");
    m_groupBoxPrefRadioStairs = new QRadioButton(fieldStairs);
    m_groupBoxPrefRadioElevator = new QRadioButton(fieldElevator);

    connect(m_groupBoxPrefRadioDefault, SIGNAL(clicked(bool)), this, SLOT(prefWasChangedSlot()));
    connect(m_groupBoxPrefRadioStairs, SIGNAL(clicked(bool)), this, SLOT(prefWasChangedSlot()));
    connect(m_groupBoxPrefRadioElevator, SIGNAL(clicked(bool)), this, SLOT(prefWasChangedSlot()));
    prefWasChangedSlot();

    m_groupBoxPrefRadioDefault->setChecked(true);
    QVBoxLayout *vboxPref = new QVBoxLayout;
    vboxPref->addWidget(m_groupBoxPrefRadioDefault);
    vboxPref->addWidget(m_groupBoxPrefRadioStairs);
    vboxPref->addWidget(m_groupBoxPrefRadioElevator);
    vboxPref->addStretch(1);
    groupBoxPref->setLayout(vboxPref);

    QVBoxLayout *groupBoxPrefLayout = new QVBoxLayout;
    groupBoxPrefLayout->addWidget(groupBoxPref);
    groupBoxPrefLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Maximum, QSizePolicy::MinimumExpanding));

    /*combos labels*/
    QLabel* currentLocationLabel = new QLabel(config->currentLocationLabelText);
    currentLocationLabel->setStyleSheet(config->globalTextAttributes + config->currentLocationLabelStyle);
    QLabel* destinationLabel = new QLabel(config->destinationLabelText);
    destinationLabel->setStyleSheet(config->globalTextAttributes + config->destinationLabelStyle);

    /*combos inside text*/
    m_currentLocationCb = new QComboBox();
    m_currentLocationCb->setStyleSheet(config->locationCBsStyle);
    connect(m_currentLocationCb, SIGNAL(activated(int)), this, SLOT(currentLocationCbHasChangedSlot()));

    m_destinationCb = new QComboBox();
    m_destinationCb->setStyleSheet(config->locationCBsStyle);
    connect(m_destinationCb, SIGNAL(activated(int)), this, SLOT(destinationCbHasChangedSlot()));

    /*combos filer*/
    for (QMap<QString,QString> room : m_roomsObjects){
        if(!m_floorToShow1.keys().contains(room[fieldFloor].toInt())){
            m_floorToShow1.insert(room[fieldFloor].toInt(),room[fieldFloor]);
            m_floorToShow2.insert(room[fieldFloor].toInt(),room[fieldFloor]);
        }
    }

    m_groupBoxCurrentLocationCbWidget = new QGroupBox(config->floorsNumbersWidgetHeader);
    m_groupBoxCurrentLocationCbWidget->setStyleSheet(config->floorsNumbersWidgetStyle);
    m_groupBoxCurrentLocationCbWidget->setFlat(true);
    QVBoxLayout *vboxFilter1 = new QVBoxLayout;
    for (int uniqueFloorNum : m_floorToShow1.keys()){
        QCheckBox *checkBox = new QCheckBox(m_floorToShow1[uniqueFloorNum]);
        checkBox->setChecked(true);
        checkBox->setObjectName("filter1");
        connect(checkBox, SIGNAL(clicked(bool)),this, SLOT(updateFilter1Slot()));
        vboxFilter1->addWidget(checkBox);
    }
    vboxFilter1->addStretch(1);
    m_groupBoxCurrentLocationCbWidget->setLayout(vboxFilter1);
    m_groupBoxCurrentLocationCbWidget->setGeometry(200, 200 ,200, 200);
    QPushButton* doneBtn1 = new QPushButton("Done");
    connect(doneBtn1, SIGNAL(clicked(bool)), this, SLOT(closeGroupBoxCurrentLocationCbWidgetSlot()));
    QHBoxLayout *doneBtn1HLayout = new QHBoxLayout;
    doneBtn1HLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::MinimumExpanding,QSizePolicy::Maximum));
    doneBtn1HLayout->addWidget(doneBtn1);
    vboxFilter1->addLayout(doneBtn1HLayout);

    QPushButton * m_currentLocationCbFilter = new QPushButton("filter floors");
    m_currentLocationCbFilter->setStyleSheet(config->filterButtonStyle);

    connect(m_currentLocationCbFilter, SIGNAL(clicked(bool)),this, SLOT(showFilter1Slot()));

    m_groupBoxdestinationCbWidget = new QGroupBox(config->floorsNumbersWidgetHeader);
    m_groupBoxdestinationCbWidget->setStyleSheet(config->floorsNumbersWidgetStyle);
    m_groupBoxdestinationCbWidget->setFlat(true);
    QVBoxLayout *vboxFilter2 = new QVBoxLayout;
    for (int uniqueFloorNum : m_floorToShow2.keys()){
        QCheckBox *checkBox = new QCheckBox(m_floorToShow2[uniqueFloorNum]);
        checkBox->setChecked(true);
        connect(checkBox, SIGNAL(clicked(bool)),this, SLOT(updateFilter2Slot()));
        checkBox->setObjectName("filter2");
        vboxFilter2->addWidget(checkBox);
    }
    vboxFilter2->addStretch(1);
    m_groupBoxdestinationCbWidget->setLayout(vboxFilter2);
    m_groupBoxdestinationCbWidget->setGeometry(200, 200 ,200, 200);
    QPushButton* doneBtn2 = new QPushButton("Done");
    connect(doneBtn2, SIGNAL(clicked(bool)), this, SLOT(closeGroupBoxdestinationCbWidgetSlot()));
    QHBoxLayout *doneBtn2HLayout = new QHBoxLayout;
    doneBtn2HLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::MinimumExpanding,QSizePolicy::Maximum));
    doneBtn2HLayout->addWidget(doneBtn2);
    vboxFilter2->addLayout(doneBtn2HLayout);

    QPushButton * m_destinationCbFilter = new QPushButton("filter floors");
    m_destinationCbFilter->setStyleSheet(config->filterButtonStyle);
    connect(m_destinationCbFilter, SIGNAL(clicked(bool)),this, SLOT(showFilter2Slot()));

    /*go button*/
    QPushButton* m_goButton = new QPushButton("GO");
    m_goButton->setStyleSheet(config->goButtonStyle);
    connect(m_goButton,SIGNAL(clicked(bool)),this,SLOT(goWasPressedSlot()));

    /*combos layout*/
    QVBoxLayout* combosLabelsLayout = new QVBoxLayout;
    combosLabelsLayout->addWidget(currentLocationLabel);
    combosLabelsLayout->addWidget(destinationLabel);

    QVBoxLayout* cBLayout = new QVBoxLayout();
    cBLayout->addWidget(m_currentLocationCb);
    cBLayout->addWidget(m_destinationCb);

    QVBoxLayout* multiCheckBoxesFiltersLayout = new QVBoxLayout();
    multiCheckBoxesFiltersLayout->addWidget(m_currentLocationCbFilter);
    multiCheckBoxesFiltersLayout->addWidget(m_destinationCbFilter);

    m_cbAndLabelsLayout = new QHBoxLayout();
    m_cbAndLabelsLayout->addLayout(combosLabelsLayout);
    m_cbAndLabelsLayout->addLayout(cBLayout);
    m_cbAndLabelsLayout->addLayout(multiCheckBoxesFiltersLayout);
    m_cbAndLabelsLayout->addWidget(m_goButton);
    m_cbAndLabelsLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::MinimumExpanding,QSizePolicy::Maximum));

    /*text box for direction and video player*/

    QLabel* logLabel = new QLabel(config->logLabelText);
    logLabel->setStyleSheet(config->globalTextAttributes + config->logLabelStyle);
    logLabel->setAlignment(Qt::AlignCenter);

    m_log = new QTextEdit();
    m_log->setStyleSheet(config->globalTextAttributes + config->logStyle);
    m_log->setReadOnly(true);

    QVBoxLayout* logLayout = new QVBoxLayout();
    logLayout->addWidget(logLabel);
    logLayout->addWidget(m_log);

    QVBoxLayout* videoLayout = new QVBoxLayout();

    m_videoWidget = new QVideoWidget;

    QPixmap pic(config->projectLogo);
    QPixmap scaled=pic.scaled ( m_videoWidget->height(), m_videoWidget->width(), Qt::KeepAspectRatio, Qt::SmoothTransformation );

    QLabel *label = new QLabel(m_videoWidget);
    label->setPixmap(scaled);
    label->move(45,100);


    QToolBar* tb = new QToolBar();

    m_rePlay = new QPushButton("rePlay");
    connect(m_rePlay, SIGNAL(clicked(bool)), this, SLOT(rePlaySlot()));
    tb->addWidget(m_rePlay);

    m_next = new QPushButton("next");
    connect(m_next, SIGNAL(clicked(bool)), this, SLOT(nextSlot()));
    tb->addWidget(m_next);
    QSizePolicy p1(QSizePolicy::Minimum,QSizePolicy::MinimumExpanding );
    m_log->setSizePolicy(p1);
    videoLayout->addWidget(m_videoWidget);
    videoLayout->addWidget(tb);
    m_logSpacerLayout = new QHBoxLayout();
    m_logSpacerLayout->addLayout(videoLayout);
    m_logSpacerLayout->addLayout(logLayout);
    m_logSpacerLayout->addLayout(groupBoxPrefLayout);
}

Nav::~Nav(){
    m_groupBoxCurrentLocationCbWidget->close();
    m_groupBoxdestinationCbWidget->close();
}
