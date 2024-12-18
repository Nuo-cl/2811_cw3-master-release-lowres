#include "pause_toggle_button.h"

PauseToggleButton::PauseToggleButton(QWidget *parent)
    : QPushButton(parent), isPaused(false),
      pauseIcon(":/icons/icons/pause_24dp_5F6368.svg"), // 更新路径
      resumeIcon(":/icons/icons/play_arrow_24dp_5F6368.svg") { // 更新路径
    setIcon(pauseIcon);
    setText("Pause");
    connect(this, &QPushButton::clicked, this, &PauseToggleButton::handleButtonClick);
}

void PauseToggleButton::handleButtonClick() {
    isPaused = !isPaused;
    setIcon(isPaused ? resumeIcon : pauseIcon);
    setText(isPaused ? "Resume" : "Pause");
    emit toggled(isPaused);
}