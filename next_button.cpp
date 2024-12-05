#include "next_button.h"

NextButton::NextButton(ThePlayer* player, QWidget* parent)
    : QPushButton("Next Video", parent), player(player) {
    connect(this, &QPushButton::clicked, this, &NextButton::handleClick);
}

void NextButton::handleClick() {
    player->playNext();
}