#include "previous_button.h"

PreviousButton::PreviousButton(ThePlayer* player, QWidget* parent)
    : QPushButton("Previous Video", parent), player(player) {
    connect(this, &QPushButton::clicked, this, &PreviousButton::handleClick);
}

void PreviousButton::handleClick() {
    player->playPrevious();
}