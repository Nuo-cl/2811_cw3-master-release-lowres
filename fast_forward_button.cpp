#include "fast_forward_button.h"

FastForwardButton::FastForwardButton(QMediaPlayer* player, PauseToggleButton* pauseButton, QWidget* parent)
    : QPushButton(parent), player(player), pauseButton(pauseButton) {
    setText("Fast Forward");
    setIcon(QIcon(":/icons/icons/fast_forward_24dp_5F6368.svg")); // 设置图标
    setIconSize(QSize(36, 36)); // 设置图标大小
    connect(this, &QPushButton::clicked, this, &FastForwardButton::handleButtonClick);
}

void FastForwardButton::handleButtonClick() {
    qint64 currentPosition = player->position();
    qint64 duration = player->duration();
    qint64 newPosition = currentPosition + 3000; // 快进3秒

    if (newPosition >= duration) {
        newPosition = duration; // 如果快进后超过视频长度，则设置为视频结束位置
        player->setPosition(newPosition);
        player->stop(); // 停止当前视频
        pauseButton->setChecked(false); // 重置暂停按钮状态
        pauseButton->setIcon(QIcon(":/icons/icons/pause_24dp_5F6368.svg")); // 重置图标为播放图标
        pauseButton->setText("Pause"); // 重置文本为 "Play"
    } else {
        player->setPosition(newPosition);
    }
}