#include "previous_button.h"

PreviousButton::PreviousButton(ThePlayer* player, QWidget* parent)
    : QPushButton("Previous Video", parent), player(player) {
    setText("Previous Video");
    setIcon(QIcon(":/icons/icons/navigate_before_24dp_5F6368.svg")); // 设置图标
    connect(this, &QPushButton::clicked, this, &PreviousButton::handleClick);
}

void PreviousButton::handleClick() {
    player->playPrevious();
}