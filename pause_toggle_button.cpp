#include "pause_toggle_button.h"

PauseToggleButton::PauseToggleButton(QWidget *parent)
    : QPushButton(parent), isPaused(false) {
    setText("Pause");
    connect(this, &QPushButton::clicked, this, &PauseToggleButton::handleButtonClick);
}

void PauseToggleButton::handleButtonClick() {
    isPaused = !isPaused;
    setText(isPaused ? "Resume" : "Pause");
    emit toggled(isPaused);
}