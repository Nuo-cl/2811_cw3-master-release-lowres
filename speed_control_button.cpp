#include "speed_control_button.h"
#include <QMessageBox>
#include <QActionGroup>

SpeedControlButton::SpeedControlButton(QMediaPlayer* player, QWidget* parent)
    : QPushButton(parent), player(player) {
    setText("Speed");
    setIcon(QIcon(":/icons/icons/rocket_launch_24dp_5F6368.svg")); // 设置图标
    setIconSize(QSize(36, 36)); // 设置图标大小

    speedMenu = new QMenu(this);
    QActionGroup* actionGroup = new QActionGroup(this);

    QAction* action05x = speedMenu->addAction("0.5x");
    QAction* action075x = speedMenu->addAction("0.75x");
    QAction* action10x = speedMenu->addAction("1.0x");
    QAction* action15x = speedMenu->addAction("1.5x");
    QAction* action20x = speedMenu->addAction("2.0x");

    actionGroup->addAction(action05x);
    actionGroup->addAction(action075x);
    actionGroup->addAction(action10x);
    actionGroup->addAction(action15x);
    actionGroup->addAction(action20x);

    action10x->setCheckable(true);
    action10x->setChecked(true);

    connect(actionGroup, &QActionGroup::triggered, this, &SpeedControlButton::setPlaybackRate);

    connect(this, &QPushButton::clicked, this, &SpeedControlButton::handleButtonClick);
}

void SpeedControlButton::handleButtonClick() {
    speedMenu->exec(mapToGlobal(QPoint(0, height())));
}

void SpeedControlButton::setPlaybackRate(QAction* action) {
    bool ok;
    qreal rate = action->text().remove('x').toDouble(&ok);
    if (ok) {
        player->setPlaybackRate(rate);
        if (player->playbackRate() != rate) {
            QMessageBox::warning(this, "Unsupported Rate", QString("Audio device or filter does not support rate: %1").arg(rate));
        } else {
            // 更新选中项的背景颜色
            for (QAction* act : speedMenu->actions()) {
                if (act == action) {
                    act->setChecked(true);
                } else {
                    act->setChecked(false);
                }
            }
        }
    }
}