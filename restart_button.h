#ifndef RESTART_BUTTON_H
#define RESTART_BUTTON_H

#include <QPushButton>
#include <QMediaPlayer>
#include "pause_toggle_button.h"

class RestartButton : public QPushButton {
    Q_OBJECT

public:
    explicit RestartButton(QMediaPlayer* player, PauseToggleButton* pauseButton, QWidget* parent = nullptr);

private slots:
    void handleButtonClick();

private:
    QMediaPlayer* player;
    PauseToggleButton* pauseButton;
};

#endif // RESTART_BUTTON_H