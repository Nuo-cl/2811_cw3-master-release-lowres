#ifndef PREVIOUS_BUTTON_H
#define PREVIOUS_BUTTON_H

#include <QPushButton>
#include "the_player.h"

class PreviousButton : public QPushButton {
    Q_OBJECT

public:
    explicit PreviousButton(ThePlayer* player, QWidget* parent = nullptr);

private slots:
    void handleClick();

private:
    ThePlayer* player;
};

#endif // PREVIOUS_BUTTON_H