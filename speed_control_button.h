#ifndef SPEED_CONTROL_BUTTON_H
#define SPEED_CONTROL_BUTTON_H

#include <QPushButton>
#include <QMediaPlayer>
#include <QMenu>

class SpeedControlButton : public QPushButton {
    Q_OBJECT

public:
    explicit SpeedControlButton(QMediaPlayer* player, QWidget* parent = nullptr);

private slots:
    void handleButtonClick();
    void setPlaybackRate(QAction* action);

private:
    QMediaPlayer* player;
    QMenu* speedMenu;
};

#endif // SPEED_CONTROL_BUTTON_H