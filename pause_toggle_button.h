#ifndef PAUSE_TOGGLE_BUTTON_H
#define PAUSE_TOGGLE_BUTTON_H

#include <QPushButton>
#include <QIcon>

class PauseToggleButton : public QPushButton {
    Q_OBJECT

public:
    explicit PauseToggleButton(QWidget *parent = nullptr);
    bool Paused();

signals:
    void toggled(bool paused);

private slots:
    void handleButtonClick();

private:
    bool isPaused;
    QIcon pauseIcon;
    QIcon resumeIcon;
};

#endif // PAUSE_TOGGLE_BUTTON_H