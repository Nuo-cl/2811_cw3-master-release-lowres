#include "timeline_slider.h"

TimelineSlider::TimelineSlider(QWidget *parent)
    : QSlider(Qt::Horizontal, parent), player(nullptr) {
    connect(this, &QSlider::sliderMoved, this, &TimelineSlider::handleSliderMoved);
}

void TimelineSlider::setDuration(qint64 duration) {
    setMaximum(static_cast<int>(duration));
}

void TimelineSlider::setPosition(qint64 position) {
    setValue(static_cast<int>(position));
}

void TimelineSlider::handleSliderMoved(int position) {
    emit sliderMoved(position);
}