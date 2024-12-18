#ifndef FAST_REWIND_BUTTON_H
#define FAST_REWIND_BUTTON_H

#include <QPushButton>
#include <QMediaPlayer>

class FastRewindButton : public QPushButton {
    Q_OBJECT

public:
    explicit FastRewindButton(QMediaPlayer* player, QWidget* parent = nullptr);

private slots:
    void handleButtonClick();

private:
    QMediaPlayer* player;
};

#endif // FAST_REWIND_BUTTON_H