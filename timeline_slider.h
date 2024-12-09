#ifndef TIMELINE_SLIDER_H
#define TIMELINE_SLIDER_H

#include <QSlider>
#include <QMediaPlayer>

class TimelineSlider : public QSlider {
    Q_OBJECT

public:
    explicit TimelineSlider(QWidget *parent = nullptr);

public slots:
    void setDuration(qint64 duration);
    void setPosition(qint64 position);

signals:
    void sliderMoved(int position);

private slots:
    void handleSliderMoved(int position);

private:
    QMediaPlayer *player;
};

#endif // TIMELINE_SLIDER_H