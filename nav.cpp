#include "nav.h"

Nav::Nav(QWidget *parent):QWidget(parent)
{
    setObjectName("Nav gui manager");
    readAndCache();
    init();
}

Nav::~Nav()
{

}



/*read files once at start up and cache them*/
void Nav::readAndCache()
{
    readRoomsFromXml();
}


void Nav::readRoomsFromXml()//temp solution
{
    //m_roomsObjects = readRoomsFromXmlUsingEngine(roomsXmlPath);
    Graph* graph = new Graph(roomsXmlPath.toUtf8().constData());
    QMap<QString,QString> room1;
    room1["name"] = "room1";
    room1["id"] = "1";
    QMap<QString,QString> room2;
    room2["name"] = "room2";
    room2["id"] = "2";
    m_roomsObjects.push_back(room1);
    m_roomsObjects.push_back(room2);
    cout << m_roomsObjects;
}

void Nav::init()
{
   /*general attributes*/
   resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
   m_mainVLayout = new QVBoxLayout;
   m_mainVLayout->setObjectName("main vertical layout");
   setLayout(m_mainVLayout);

   /*widow title*/
   setWindowTitle(windowTitleText);

   /*project title*/
    QLabel* projectLabel = new QLabel(projectTitleLabelText);
    projectLabel->setStyleSheet(globalTextAttributes + projectTitleLabelStyle);
    projectLabel->setAlignment(Qt::AlignCenter);
    projectLabel->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Maximum);

    QHBoxLayout* titleLayout = new QHBoxLayout();
    titleLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::MinimumExpanding,QSizePolicy::Maximum));
    titleLayout->addWidget(projectLabel);
    titleLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::MinimumExpanding,QSizePolicy::Maximum));

    m_mainVLayout->addLayout(titleLayout);

   /*control buttons*/
   m_resetButton = new QPushButton("reset");
   connect(m_resetButton,SIGNAL(clicked(bool)),this,SLOT(resetSlot()));
   QHBoxLayout* controlButtonsLayout = new QHBoxLayout();
   controlButtonsLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::MinimumExpanding,QSizePolicy::Maximum));
   controlButtonsLayout->addWidget(m_resetButton);
   m_mainVLayout->addLayout(controlButtonsLayout);


    /*combos labels*/
    QLabel* currentLocationLabel = new QLabel(currentLocationLabelText);
    currentLocationLabel->setStyleSheet(globalTextAttributes + currentLocationLabelStyle);
    QLabel* destinationLabel = new QLabel(destinationLabelText);
    destinationLabel->setStyleSheet(globalTextAttributes + destinationLabelStyle);

    /*combos inside text*/
    m_currentLocationCb = new QComboBox();
    QStringList currentLocationCbTexts;
    for(QMap<QString,QString> room : m_roomsObjects){
        currentLocationCbTexts.push_back(room[comboKey]);
    }
    m_currentLocationCb->addItems(currentLocationCbTexts);
    connect(m_currentLocationCb, SIGNAL(activated(int)), this, SLOT(currentLocationCbHasChangedSlot(int)));

    m_destinationCb = new QComboBox();
    QStringList destinationCbTexts;
    for(QMap<QString,QString> room : m_roomsObjects){
        destinationCbTexts.push_back(room[comboKey]);
    }
    m_destinationCb->addItems(destinationCbTexts);
    connect(m_destinationCb, SIGNAL(activated(int)), this, SLOT(destinationCbHasChangedSlot(int)));

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

    QHBoxLayout* cbAndLabelsLayout = new QHBoxLayout();
    cbAndLabelsLayout->addLayout(combosLabelsLayout);
    cbAndLabelsLayout->addLayout(cBLayout);
    cbAndLabelsLayout->addWidget(m_goButton);
    cbAndLabelsLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::MinimumExpanding,QSizePolicy::Maximum));

    m_mainVLayout->addLayout(cbAndLabelsLayout);

    /*text box for direction. will be replaced in video player in the future*/

    QLabel* logLabel = new QLabel(logLabelText);
    logLabel->setStyleSheet(globalTextAttributes + logLabelStyle);
    logLabel->setAlignment(Qt::AlignCenter);

    m_log = new QTextEdit();
    m_log->setStyleSheet(globalTextAttributes + logStyle);
    m_log->setReadOnly(true);
    connect(this, SIGNAL(appendToLogSignal(QString)), this, SLOT(appendToLogSlot(QString)));

    QVBoxLayout* logLayout = new QVBoxLayout();
    logLayout->addWidget(logLabel);
    logLayout->addWidget(m_log);

    QHBoxLayout* logSpacerLayout = new QHBoxLayout();
    logSpacerLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::MinimumExpanding,QSizePolicy::Maximum));
    logSpacerLayout->addLayout(logLayout);
    logSpacerLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::MinimumExpanding,QSizePolicy::Maximum));
    m_mainVLayout->addLayout(logSpacerLayout);
}

void Nav::resetSlot()
{
    disconnect(m_resetButton,SIGNAL(clicked(bool)),this,SLOT(resetSlot()));
    disconnect(m_currentLocationCb, SIGNAL(activated(int)), this, SLOT(currentLocationCbHasChangedSlot(int)));
    disconnect(m_destinationCb, SIGNAL(activated(int)), this, SLOT(destinationCbHasChangedSlot(int)));
    disconnect(m_goButton,SIGNAL(clicked(bool)),this,SLOT(goWasPressedSlot()));
    disconnect(this, SIGNAL(appendToLogSignal(QString)), this, SLOT(appendToLogSlot(QString)));

    delete m_log;
    delete m_goButton;
    delete m_destinationCb;
    delete m_currentLocationCb;
    delete m_resetButton;
    delete m_mainVLayout;
    init();
}

void Nav::currentLocationCbHasChangedSlot(int index)
{
    cout << index;
    //emit appendToLogSignal(m_currentLocationCb->itemText(index));
}

void Nav::destinationCbHasChangedSlot(int index)
{
    cout << index;
    //emit appendToLogSignal(m_currentLocationCb->itemText(index));
}

void Nav::goWasPressedSlot()
{
    m_log->clear();
    m_log->append( "route from " + m_currentLocationCb->currentText() + " to " + m_destinationCb->currentText() + ": " );
}

void Nav::appendToLogSlot(QString str)
{
    cout << str;
    m_log->append(str);
}
