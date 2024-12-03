#ifndef NEXT_BUTTON_H
#define NEXT_BUTTON_H

#include <QPushButton>
#include "the_player.h"

class NextButton : public QPushButton {
    Q_OBJECT

public:
    NextButton(ThePlayer* player, QWidget* parent = nullptr);

private slots:
    void handleClick();

private:
    ThePlayer* player;
};

#endif // NEXT_BUTTON_H