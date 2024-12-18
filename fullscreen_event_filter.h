#ifndef FULLSCREEN_EVENT_FILTER_H
#define FULLSCREEN_EVENT_FILTER_H

#include <QObject>
#include <QPushButton>
#include <QVBoxLayout>
#include <QKeyEvent>

class FullscreenEventFilter : public QObject {
    Q_OBJECT
public:
    FullscreenEventFilter(bool* isFullscreen, QWidget* window, QVBoxLayout* top, 
                          QWidget* videoWidget, QWidget* timelineSlider, QWidget* controlWidget,
                          QWidget* pauseVolumeWidget, QWidget* buttonWidget, QWidget* navigatorWidget,
                          QPushButton* languageButton, QPushButton* fullscreenButton)
        : isVideoFullscreen(isFullscreen), window(window), top(top), videoWidget(videoWidget),
          timelineSlider(timelineSlider), controlWidget(controlWidget),
          pauseVolumeWidget(pauseVolumeWidget), buttonWidget(buttonWidget),
          navigatorWidget(navigatorWidget), languageButton(languageButton),
          fullscreenButton(fullscreenButton) {}

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    bool* isVideoFullscreen;
    QWidget* window;
    QVBoxLayout* top;
    QWidget* videoWidget;
    QWidget* timelineSlider;
    QWidget* controlWidget;
    QWidget* pauseVolumeWidget;
    QWidget* buttonWidget;
    QWidget* navigatorWidget;
    QPushButton* languageButton;
    QPushButton* fullscreenButton;
};

#endif // FULLSCREEN_EVENT_FILTER_H 