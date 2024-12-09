#ifndef VOLUME_BUTTON_H
#define VOLUME_BUTTON_H

#include <QSlider>
#include <QMediaPlayer>

class VolumeButton : public QSlider {
    Q_OBJECT

public:
    explicit VolumeButton(QWidget *parent = nullptr);

signals:
    void volumeChanged(int volume);

private slots:
    void handleSliderMoved(int position);

private:
    QMediaPlayer *player;
};

#endif // VOLUME_BUTTON_H