//
//
//

#ifndef CW2_THE_PLAYER_H
#define CW2_THE_PLAYER_H


#include <QApplication>
#include <QMediaPlayer>
#include "the_button.h"
#include <vector>
#include <QTimer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>

class ThePlayer : public QMediaPlayer {

Q_OBJECT

private:
    std::vector<TheButtonInfo>* infos;
    std::vector<TheButton*>* buttons;
    QTimer* mTimer;
    long updateCount = 0;
    int currentIndex = 0;

    // 数据库相关
    QSqlDatabase db;

public:
    ThePlayer() : QMediaPlayer(NULL) {
        setVolume(1); // 调整音量
        connect (this, SIGNAL (stateChanged(QMediaPlayer::State)), this, SLOT (playStateChanged(QMediaPlayer::State)) ); // if state changes, run playStateChanged

        mTimer = new QTimer(NULL);
        mTimer->setInterval(1000); // 每秒一次
        mTimer->start();
        // connect( mTimer, SIGNAL (timeout()), SLOT ( shuffle() ) ); // ...running shuffle method
        
        // 初始化数据库
        initializeDatabase();
    }

    // all buttons have been setup, store pointers here
    void setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i);

private slots:

    // change the image and video for one button every one second
    void shuffle();
    void playStateChanged (QMediaPlayer::State ms);

public slots:

    // start playing this ButtonInfo
    void jumpTo (TheButtonInfo* button);
    void playNext();
    void playPrevious();
    void handlePauseToggle(bool paused);

    // 新增的槽函数
    void likeCurrentVideo();
    void favoriteCurrentVideo();

signals:
    // 新增信号以通知点赞和收藏状态的变化
    void likeStatusChanged(bool liked);
    void favoriteStatusChanged(bool favorited);
    void videoPathChanged(const QString& path);

private:
    void initializeDatabase();
    void likeVideo(const QString& videoPath);
    void favoriteVideo(const QString& videoPath);

    // 新增方法以查询视频是否被点赞或收藏
    bool isVideoLiked(const QString& videoPath);
    bool isVideoFavorited(const QString& videoPath);
};

#endif //CW2_THE_PLAYER_H
