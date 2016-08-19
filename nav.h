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


class Nav : public QWidget
{
    Q_OBJECT

public:
    explicit Nav(QWidget *parent = 0);
    ~Nav();
    void readAndCache();
    void readRoomsFromXml();
    void init();

public slots:
    void resetSlot();
    void currentLocationCbHasChangedSlot(int index);
    void destinationCbHasChangedSlot(int index);
    void goWasPressedSlot();
    void appendToLogSlot(QString str);

signals:
    void appendToLogSignal(QString);

private:
    QList<QMap<QString,QString>> m_roomsObjects;//each room object got a list of attributes
    QVBoxLayout* m_mainVLayout;
    QPushButton* m_resetButton;
    QComboBox * m_currentLocationCb;
    QComboBox * m_destinationCb;
    QPushButton* m_goButton;
    QTextEdit* m_log;
};

#endif // NAV_H
