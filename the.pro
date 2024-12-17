QT += core gui widgets multimedia multimediawidgets

CONFIG += c++11


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    fast_forward_button.cpp \
    fast_rewind_button.cpp \
    restart_button.cpp \
    speed_control_button.cpp \
    the_button.cpp \
    the_player.cpp \
    button_navigator.cpp\
    next_button.cpp \
    previous_button.cpp \
    tomeo.cpp \
    pause_toggle_button.cpp \
    timeline_slider.cpp \
    volume_button.cpp

HEADERS += \
    fast_forward_button.h \
    fast_rewind_button.h \
    restart_button.h \
    speed_control_button.h \
    the_button.h \
    the_player.h \
    next_button.h \
    previous_button.h \
    button_navigator.h \
    pause_toggle_button.h \
    timeline_slider.h \
    volume_button.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

