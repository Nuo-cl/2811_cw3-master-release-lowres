//
//
//

#include "the_player.h"
#include <QSqlError>
#include <QDebug>

// all buttons have been setup, store pointers here
void ThePlayer::setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i) {
    buttons = b;
    infos = i;
    jumpTo(buttons -> at(0) -> info);
}

// change the image and video for one button every one second
void ThePlayer::shuffle() {
    TheButtonInfo* i = & infos -> at (rand() % infos->size() );
//        setMedia(*i->url);
    buttons -> at( updateCount++ % buttons->size() ) -> init( i );
}

void ThePlayer::playStateChanged (QMediaPlayer::State ms) {
    switch (ms) {
        case QMediaPlayer::State::StoppedState:
            play(); // starting playing again...
            break;
    default:
        break;
    }
}

void ThePlayer::jumpTo (TheButtonInfo* buttonInfo) {
    setMedia( * buttonInfo -> url);
    play();

    QString videoPath = buttonInfo->url->toLocalFile();
    bool liked = isVideoLiked(videoPath);
    bool favorited = isVideoFavorited(videoPath);

    emit likeStatusChanged(liked);
    emit favoriteStatusChanged(favorited);
    emit videoPathChanged(videoPath);
}

void ThePlayer::playNext() {
    currentIndex = (currentIndex + 1) % buttons -> size();
    jumpTo(buttons -> at(currentIndex) -> info);
}

void ThePlayer::playPrevious() {
    currentIndex = (currentIndex - 1 + buttons -> size()) % buttons -> size();
    jumpTo(buttons -> at(currentIndex) -> info);
}

void ThePlayer::handlePauseToggle(bool paused) {
    if (paused) {
        pause();
    } else {
        play();
    }
}

void ThePlayer::initializeDatabase() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("video_data.db");

    if (!db.open()) {
        qDebug() << "无法打开数据库:" << db.lastError().text();
        return;
    }

    QSqlQuery query;
    // 创建表格，如果不存在的话
    QString createTable = "CREATE TABLE IF NOT EXISTS VideoStatus ("
                          "Path TEXT PRIMARY KEY, "
                          "Liked INTEGER DEFAULT 0, "
                          "Favorited INTEGER DEFAULT 0)";
    if (!query.exec(createTable)) {
        qDebug() << "创建表格失败:" << query.lastError().text();
    }
}

void ThePlayer::likeVideo(const QString& videoPath) {
    QSqlQuery query;
    // 检查视频是否已存在
    query.prepare("SELECT Liked FROM VideoStatus WHERE Path = :path");
    query.bindValue(":path", videoPath);
    if (query.exec()) {
        if (query.next()) {
            // 更新点赞状态
            bool currentStatus = query.value(0).toInt();
            bool newStatus = !currentStatus;
            query.prepare("UPDATE VideoStatus SET Liked = :liked WHERE Path = :path");
            query.bindValue(":liked", newStatus ? 1 : 0);
            query.bindValue(":path", videoPath);
            if (!query.exec()) {
                qDebug() << "更新点赞状态失败:" << query.lastError().text();
            }
            // 发射点赞状态变化信号
            emit likeStatusChanged(newStatus);
        } else {
            // 插入新记录
            query.prepare("INSERT INTO VideoStatus (Path, Liked) VALUES (:path, 1)");
            query.bindValue(":path", videoPath);
            if (!query.exec()) {
                qDebug() << "插入点赞记录失败:" << query.lastError().text();
            }
            // 发射点赞状态变化信号
            emit likeStatusChanged(true);
        }
    } else {
        qDebug() << "查询失败:" << query.lastError().text();
    }
}

void ThePlayer::favoriteVideo(const QString& videoPath) {
    QSqlQuery query;
    // 检查视频是否已存在
    query.prepare("SELECT Favorited FROM VideoStatus WHERE Path = :path");
    query.bindValue(":path", videoPath);
    if (query.exec()) {
        if (query.next()) {
            // 更新收藏状态
            bool currentStatus = query.value(0).toInt();
            bool newStatus = !currentStatus;
            query.prepare("UPDATE VideoStatus SET Favorited = :favorited WHERE Path = :path");
            query.bindValue(":favorited", newStatus ? 1 : 0);
            query.bindValue(":path", videoPath);
            if (!query.exec()) {
                qDebug() << "更新收藏状态失败:" << query.lastError().text();
            }
            // 发射收藏状态变化信号
            emit favoriteStatusChanged(newStatus);
        } else {
            // 插入新记录
            query.prepare("INSERT INTO VideoStatus (Path, Favorited) VALUES (:path, 1)");
            query.bindValue(":path", videoPath);
            if (!query.exec()) {
                qDebug() << "插入收藏记录失败:" << query.lastError().text();
            }
            // 发射收藏状态变化信号
            emit favoriteStatusChanged(true);
        }
    } else {
        qDebug() << "查询失败:" << query.lastError().text();
    }
}

void ThePlayer::likeCurrentVideo() {
    if (buttons && buttons->size() > currentIndex) {
        QString currentVideoPath = infos->at(currentIndex).url->toLocalFile();
        likeVideo(currentVideoPath);
        // 这里可以添加UI更新，例如改变按钮颜色
    }
}

void ThePlayer::favoriteCurrentVideo() {
    if (buttons && buttons->size() > currentIndex) {
        QString currentVideoPath = infos->at(currentIndex).url->toLocalFile();
        favoriteVideo(currentVideoPath);
        // 这里可以添加UI更新，例如改变按钮颜色
    }
}

// 实现查询视频是否被点赞的方法
bool ThePlayer::isVideoLiked(const QString& videoPath) {
    QSqlQuery query;
    query.prepare("SELECT Liked FROM VideoStatus WHERE Path = :path");
    query.bindValue(":path", videoPath);
    if (query.exec()) {
        if (query.next()) {
            return query.value(0).toInt() == 1;
        }
    }
    return false;
}

// 实现查询视频是否被收藏的方法
bool ThePlayer::isVideoFavorited(const QString& videoPath) {
    QSqlQuery query;
    query.prepare("SELECT Favorited FROM VideoStatus WHERE Path = :path");
    query.bindValue(":path", videoPath);
    if (query.exec()) {
        if (query.next()) {
            return query.value(0).toInt() == 1;
        }
    }
    return false;
}