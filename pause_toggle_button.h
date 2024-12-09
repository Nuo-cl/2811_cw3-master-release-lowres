#ifndef PAUSE_TOGGLE_BUTTON_H
#define PAUSE_TOGGLE_BUTTON_H

#include <QPushButton>

class PauseToggleButton : public QPushButton {
    Q_OBJECT

public:
    explicit PauseToggleButton(QWidget *parent = nullptr);

signals:
    void toggled(bool paused);

private slots:
    void handleButtonClick();

private:
    bool isPaused;
};

#endif // PAUSE_TOGGLE_BUTTON_H