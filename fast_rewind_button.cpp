#include "fast_rewind_button.h"

FastRewindButton::FastRewindButton(QMediaPlayer* player, QWidget* parent)
    : QPushButton(parent), player(player) {
    setText("Fast Rewind");
    setIcon(QIcon(":/icons/icons/fast_rewind_24dp_5F6368.svg")); // 设置图标
    setIconSize(QSize(36, 36)); // 设置图标大小
    connect(this, &QPushButton::clicked, this, &FastRewindButton::handleButtonClick);
}

void FastRewindButton::handleButtonClick() {
    qint64 currentPosition = player->position();
    qint64 newPosition = currentPosition - 3000; // 快退3秒

    if (newPosition < 0) {
        newPosition = 0; // 如果快退后小于视频开始位置，则设置为视频开始位置
    }

    player->setPosition(newPosition);
}