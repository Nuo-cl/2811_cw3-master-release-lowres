#include "volume_button.h"

VolumeButton::VolumeButton(QWidget *parent)
    : QSlider(Qt::Horizontal, parent), player(nullptr) {
    setRange(0, 100); // Volume range from 0 to 100
    setValue(50); // Default volume level to 50
    connect(this, &QSlider::sliderMoved, this, &VolumeButton::handleSliderMoved);
}

void VolumeButton::handleSliderMoved(int position) {
    emit volumeChanged(position);
}