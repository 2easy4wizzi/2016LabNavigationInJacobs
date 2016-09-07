#include "nav.h"

Nav::Nav(QWidget *parent):QWidget(parent)
{
    setObjectName("Nav gui manager");
    m_roomVideoDisplay = NULL;
    m_shortestPathQt.clear();
    readRoomsFromXml();
    initOnce();
    init();
}

Nav::~Nav(){
    m_groupBoxCurrentLocationCbWidget->close();
    m_groupBoxdestinationCbWidget->close();
}

void Nav::readRoomsFromXml()
{
    bool succesReadingXml;
    m_graph = new Graph(roomsXmlPath.toUtf8().constData(), edgesXmlPath.toUtf8().constData(), succesReadingXml);
    if (!succesReadingXml){
        exitProgramWithErrMsg("Unable to read XML files.\nPlease check their paths in config.h are valid");
    }
    translateRoomsFromCppToQt();
}

//creating 2 containers.
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
        const pair<Direction, int> * neighbos = node->GetNeihbors();
        for (int i=0; i<NUM_OF_NEIGBHORS ; i++){
            room[dirMap2[neighbos[i].first]] = QString::number(neighbos[i].second);
        }
        const int * classes = node->GetClasses();
        for (int i=0; i<NUMBER_OF_NEIGBHORS ; i++){
            room["Class" + QString::number(i)] = QString::number(classes[i]);
        }
        room[fieldFloor] = QString::number(node->GetNodeFloor());
        room[fieldNumber] = (node->GetNumber().c_str());
        room[fieldStartIndex] = QString::number(node->GetVideoStartIndex());
        room[fieldEndIndex] = QString::number(node->GetVideoEndIndex());
        room[fieldVideoPath] = node->GetVideoPath().c_str();
        m_roomsObjects.push_back(room);
    }
    cout << m_roomsObjects;
}

//happens once
void Nav::initOnce()
{
    /*general attributes*/
    this->setFixedSize(QSize(1280,720));

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
    m_resetButton->setStyleSheet(resetButtonStyle);
    connect(m_resetButton,SIGNAL(clicked(bool)),this,SLOT(resetSlot()));


    m_controlButtonsHLayout = new QHBoxLayout();
    m_controlButtonsHLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::MinimumExpanding,QSizePolicy::Maximum));
    m_controlButtonsHLayout->addWidget(m_resetButton);


    /*group box preference - elevator vs stairs*/
    QGroupBox *groupBoxPref = new QGroupBox("Take me from the");
    groupBoxPref->setStyleSheet(groupBoxPrefStyle);
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
    QLabel* currentLocationLabel = new QLabel(currentLocationLabelText);
    currentLocationLabel->setStyleSheet(globalTextAttributes + currentLocationLabelStyle);
    QLabel* destinationLabel = new QLabel(destinationLabelText);
    destinationLabel->setStyleSheet(globalTextAttributes + destinationLabelStyle);

    /*combos inside text*/
    m_currentLocationCb = new QComboBox();
    m_currentLocationCb->setStyleSheet(locationCBsStyle);
    connect(m_currentLocationCb, SIGNAL(activated(int)), this, SLOT(currentLocationCbHasChangedSlot()));

    m_destinationCb = new QComboBox();
    m_destinationCb->setStyleSheet(locationCBsStyle);
    connect(m_destinationCb, SIGNAL(activated(int)), this, SLOT(destinationCbHasChangedSlot()));

    /*combos filer*/


    for (QMap<QString,QString> room : m_roomsObjects){
        if(!m_floorToShow1.keys().contains(room[fieldFloor].toInt())){
            m_floorToShow1.insert(room[fieldFloor].toInt(),room[fieldFloor]);
            m_floorToShow2.insert(room[fieldFloor].toInt(),room[fieldFloor]);
        }
    }

    m_groupBoxCurrentLocationCbWidget = new QGroupBox(floorsNumbersWidgetHeader);
    m_groupBoxCurrentLocationCbWidget->setStyleSheet(floorsNumbersWidgetStyle);
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

    m_currentLocationCbFilter = new QPushButton("filter floors");
    m_currentLocationCbFilter->setStyleSheet(filterButtonStyle);

    connect(m_currentLocationCbFilter, SIGNAL(clicked(bool)),this, SLOT(showFilter1Slot()));


    m_groupBoxdestinationCbWidget = new QGroupBox(floorsNumbersWidgetHeader);
    m_groupBoxdestinationCbWidget->setStyleSheet(floorsNumbersWidgetStyle);
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

    m_destinationCbFilter = new QPushButton("filter floors");
    m_destinationCbFilter->setStyleSheet(filterButtonStyle);
    connect(m_destinationCbFilter, SIGNAL(clicked(bool)),this, SLOT(showFilter2Slot()));


    /*go button*/
    m_goButton = new QPushButton("GO");
    m_goButton->setStyleSheet(goButtonStyle);
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

    QLabel* logLabel = new QLabel(logLabelText);
    logLabel->setStyleSheet(globalTextAttributes + logLabelStyle);
    logLabel->setAlignment(Qt::AlignCenter);

    m_log = new QTextEdit();
    m_log->setStyleSheet(globalTextAttributes + logStyle);
    m_log->setReadOnly(true);




    QVBoxLayout* logLayout = new QVBoxLayout();
    logLayout->addWidget(logLabel);
    logLayout->addWidget(m_log);


    QVBoxLayout* videoLayout = new QVBoxLayout();

    m_videoWidget = new QVideoWidget;

    QPixmap pic(projectLogo);
    QPixmap scaled=pic.scaled ( m_videoWidget->height(), m_videoWidget->width(), Qt::KeepAspectRatio, Qt::SmoothTransformation );

    QLabel *label = new QLabel(m_videoWidget);
    label->setPixmap(scaled);
    label->move(45,100);
    //label->setAlignment(Qt::AlignCenter);
    //label->setPixmap(QPixmap::fromMimeSource("someimg.png"));


    QToolBar* tb = new QToolBar();

    QPushButton* rePlay = new QPushButton("rePlay");
    connect(rePlay, SIGNAL(clicked(bool)), this, SLOT(replaySlot()));
    tb->addWidget(rePlay);

    QPushButton* next = new QPushButton("next");
    connect(next, SIGNAL(clicked(bool)), this, SLOT(nextSlot()));
    tb->addWidget(next);
    //m_videoWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
    QSizePolicy p1(QSizePolicy::Minimum,QSizePolicy::MinimumExpanding );
    m_log->setSizePolicy(p1);
    videoLayout->addWidget(m_videoWidget);
    videoLayout->addWidget(tb);


    m_logSpacerLayout = new QHBoxLayout();
    m_logSpacerLayout->addLayout(videoLayout);
    m_logSpacerLayout->addLayout(logLayout);
    m_logSpacerLayout->addLayout(groupBoxPrefLayout);
    //m_logSpacerLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::MinimumExpanding,QSizePolicy::Maximum));



}

//could happen more than once(for example: if you press 'reset' button)
void Nav::init()
{
    m_mediaPlayer = new QMediaPlayer;
    m_mediaPlayer->setAudioRole(QAudio::VideoRole);
    m_mediaPlayer->setVideoOutput(m_videoWidget); //where to stream the video
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
    delete m_mediaPlayer;
    delete m_mainVLayout;

    init();
}

//given an attribute(QString), go over m_roomsObjects and look for it. return the Node with the name==str.
Node *Nav::findNodeByStr(QString str)
{
    int id = -1;
    for(QMap<QString,QString> room : m_roomsObjects)
    {
        if(str == room[fieldName] || str == (room[fieldName] + "-" +room[fieldNumber])){
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
        m_roomVideoDisplay = m_shortestPathQt.at(0);
        playVideoFromTo();//plays a video's part(could be 2 seconds from 10 seconds video) given a Node
    }
}

void Nav::translateShortestPathFromCppToQt(){
    list<Node*> shortestPath = m_graph->GetShortestpath(m_currentRoom, m_destRoom, m_pref);//graph::GetShortestpath
    for(Node* room : shortestPath){

        m_shortestPathQt.push_back(room);
    }
    if(DEBUG) printShortestPath();
}

void Nav::printShortestPath()//debuging function
{
    for (Node* room : m_shortestPathQt)
    {
        qDebug() << room->ToString().c_str();
    }
}

void Nav::appendShortestPathToLog(QString color) // 3 cases. elevator, stairs and regular. print by case.
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
    int nextRoomInPathId = roomInPath->_roomPathNextRoomInPathId;

    if(roomName.contains(fieldElevator) && getRoomFieldById(nextRoomInPathId, fieldName).contains(fieldElevator)){
        int currentFloor = roomInPath->GetNodeFloor();
        int desFloor = getRoomFieldById(nextRoomInPathId,fieldFloor).toInt();
        textToAppend = currentFloor > desFloor ?
                    (QString::number(++i) + ") Use the elevator to go down to floor " + QString::number(desFloor) + ".") :
                    (QString::number(++i) + ") Use the elevator to go up to floor "   + QString::number(desFloor) + ".");
        m_log->append(textToAppend);
    }
    else if(roomName.contains(fieldStairs)&& getRoomFieldById(nextRoomInPathId, fieldName).contains(fieldStairs)){
        int currentFloor = roomInPath->GetNodeFloor();
        int desFloor = getRoomFieldById(nextRoomInPathId,fieldFloor).toInt();
        textToAppend = currentFloor > desFloor ?
                    (QString::number(++i) + ") Go down the stairs to floor " + QString::number(desFloor) + ".") :
                    (QString::number(++i) + ") Go up the stairs to floor "   + QString::number(desFloor) + ".");
        m_log->append(textToAppend);
    }
    else{
        QString dest(getRoomFieldById(nextRoomInPathId,fieldName));
        textToAppend = QString::number(++i) + ") Walk " + QString::number(roomInPath->_roomPathDistance) +
                " meters " + roomInPath->_roomPathDirection.c_str() +
                " to " + dest + "." ;
        m_log->append(textToAppend);
    }
}


void Nav::playVideoFromTo(bool replay)//play Node's video part
{
    int startIndex = m_roomVideoDisplay->GetVideoStartIndex();
    int endIndex = m_roomVideoDisplay->GetVideoEndIndex();
    QString videoPath = m_roomVideoDisplay->GetVideoPath().c_str();

    m_mediaPlayer->setMedia(QUrl::fromLocalFile(videoPath)); //video location
    m_mediaPlayer->setPosition(startIndex); // starting index time
    if(!replay)
    {
        appendShortestPathToLog("red");
    }

    m_videoWidget->show();
    m_mediaPlayer->play();

    if(endIndex != -1)
    {
        QTimer::singleShot(endIndex, m_mediaPlayer, SLOT(pause()));
    }


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

//the next 4 functions/slots are for filtering the floors. 2 for current location and 2 for destanation
void Nav::showFilter1Slot()
{
    m_groupBoxCurrentLocationCbWidget->show();
}

void Nav::showFilter2Slot()
{
    m_groupBoxdestinationCbWidget->show();
}

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
            if(checkbox->text() == baseFloor){
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
            if(checkbox->text() == baseFloor){
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
void Nav::replaySlot()
{
    if(m_roomVideoDisplay)
    {
        playVideoFromTo(true);
    }
}

void Nav::nextSlot()
{
    if(m_roomVideoDisplay)
    {
        QTextCursor cursor = m_log->textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.select(QTextCursor::LineUnderCursor);
        cursor.removeSelectedText();
        cursor.deletePreviousChar(); // Added to trim the newline char when removing last line
        m_log->setTextCursor(cursor);
        appendShortestPathToLog();
        Node* tmpRoom = NULL;
        for (int i=0; i < m_shortestPathQt.size(); ++i)
        {
            if(m_shortestPathQt.at(i) == m_roomVideoDisplay && (i<m_shortestPathQt.size()-1) )
            {
                tmpRoom = m_shortestPathQt.at(i + 1);
                break;
            }
        }
        m_roomVideoDisplay = tmpRoom;
        if(tmpRoom)
        {
            playVideoFromTo();
        }
        else{
            delete m_mediaPlayer;
            m_mediaPlayer = new QMediaPlayer;
            m_mediaPlayer->setAudioRole(QAudio::VideoRole);
            m_mediaPlayer->setVideoOutput(m_videoWidget); //where to stream the video
            m_log->append("\nThat was the last step.\nClick on Go to repeat the instructions");
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
                //tags.push_back(room[fieldName]);
                sortKeyAndTags.insert(room[fieldFloor].toInt()*1000-(++i), room[fieldName]);
            }
            else if(room[fieldNumber] == "-1"){ //rooms that help on the route but are not a destanation
                //do nothing
            }
            else{
                QString tag = room[fieldName].contains(room[fieldNumber]) ? room[fieldName] : room[fieldName] + "-" + room[fieldNumber];
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
    err.setText(msg);
    err.exec();
}
