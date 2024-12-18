#include "fullscreen_event_filter.h"

bool FullscreenEventFilter::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(event);
        if (keyEvent && keyEvent->key() == Qt::Key_Escape && *isVideoFullscreen) {
            // 退出全屏模式
            // 显示其他控件
            timelineSlider->setVisible(true);
            controlWidget->setVisible(true);
            pauseVolumeWidget->setVisible(true);
            buttonWidget->setVisible(true);
            navigatorWidget->setVisible(true);
            languageButton->setVisible(true);
            
            // 恢复 videoWidget 的原始位置
            top->removeWidget(videoWidget);
            videoWidget->setParent(buttonWidget);
            top->insertWidget(0, videoWidget); // 插入回原来的位置
            
            // 修改按钮文本为“全屏”
            fullscreenButton->setText("全屏");
            
            *isVideoFullscreen = false;
            return true; // 事件已处理
        }
    }
    // 其他事件继续传递
    return QObject::eventFilter(obj, event);
} 