#include "next_button.h"

NextButton::NextButton(ThePlayer* player, QWidget* parent)
    : QPushButton(parent), player(player) {
    setText("Next Video");
    setIcon(QIcon(":/icons/icons/navigate_next_24dp_5F6368.svg")); // 设置图标
    setLayoutDirection(Qt::RightToLeft); // 设置布局方向，使图标在文本右侧
    connect(this, &QPushButton::clicked, this, &NextButton::handleClick);
}

void NextButton::handleClick() {
    player->playNext();
}