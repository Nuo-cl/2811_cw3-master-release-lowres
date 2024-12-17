#include "restart_button.h"

RestartButton::RestartButton(QMediaPlayer* player, PauseToggleButton* pauseButton, QWidget* parent)
    : QPushButton(parent), player(player), pauseButton(pauseButton) {
    setText("Replay");
    setIcon(QIcon(":/icons/icons/restart_alt_24dp_5F6368.svg")); // 设置图标
    setIconSize(QSize(36, 36)); // 设置图标大小
    connect(this, &QPushButton::clicked, this, &RestartButton::handleButtonClick);
}

void RestartButton::handleButtonClick() {
    player->setPosition(0); // 将视频位置设置为开始位置
    player->play(); // 播放视频

    // 重置暂停按钮状态
    pauseButton->setChecked(false);
    pauseButton->setIcon(QIcon(":/icons/icons/pause_24dp_5F6368.svg"));
    pauseButton->setText("Pause");
}