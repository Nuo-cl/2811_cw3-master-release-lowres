#ifndef FAST_FORWARD_BUTTON_H
#define FAST_FORWARD_BUTTON_H

#include <QPushButton>
#include <QMediaPlayer>
#include "pause_toggle_button.h"

class FastForwardButton : public QPushButton {
    Q_OBJECT

public:
    explicit FastForwardButton(QMediaPlayer* player, PauseToggleButton* pauseButton, QWidget* parent = nullptr);

private slots:
    void handleButtonClick();

private:
    QMediaPlayer* player;
    PauseToggleButton* pauseButton;
};

#endif // FAST_FORWARD_BUTTON_H