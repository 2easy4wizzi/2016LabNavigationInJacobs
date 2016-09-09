// https://www.youtube.com/watch?v=tGKmQy-VBX0

#include "dialog.h"
#include <QApplication>
#include <QObject>
#include <QMediaPlayer>
#include <qvideowidget.h>
#include <QDebug>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
  //  Dialog w;
   // w.show();

    QMediaPlayer* player = new QMediaPlayer;
    QVideoWidget* vw = new QVideoWidget;
    vw->setGeometry(100,100,600,400);
    vw->show();

    player->setVideoOutput(vw); //where to stream the video
    player->setMedia(QUrl::fromLocalFile("C:/Users/Adam/My Series 1/Game.of.Thrones.S01.720p.BluRay.x264.ShAaNiG/Game.of.Thrones.S01E01.720p.BluRay.500MB.ShAaNiG.com.mkv")); //video location
    player->setPosition(80000); // starting index time


    player->play();

    qDebug() << player->state();

    return a.exec();
}


