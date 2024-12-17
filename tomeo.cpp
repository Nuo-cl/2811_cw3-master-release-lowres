//
//    ______
//   /_  __/___  ____ ___  ___  ____
//    / / / __ \/ __ `__ \/ _ \/ __ \
//   / / / /_/ / / / / / /  __/ /_/ /
//  /_/  \____/_/ /_/ /_/\___/\____/
//              video for sports enthusiasts...
//
//

#include <QApplication>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaPlaylist>
#include <string>
#include <vector>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include "the_player.h"
#include "the_button.h"
#include "button_navigator.h"
// #include "control_buttons.h"
#include "previous_button.h"
#include "next_button.h"
#include "pause_toggle_button.h"
#include <QObject>
#include "timeline_slider.h"
#include "volume_button.h"
#include "restart_button.h"
#include "fast_forward_button.h"
#include "fast_rewind_button.h"
#include "speed_control_button.h"

// Function to rearrange buttons
// void rearrangeButtons(std::vector<TheButton*>& buttons, std::vector<TheButtonInfo>& videos, int startIndex, int buttonsPerPage) {
//     for (int i = 0; i < buttons.size(); ++i) {
//         if (i >= startIndex && i < startIndex + buttonsPerPage) {
//             buttons[i]->init(&videos.at(i));
//             buttons[i]->show();
//         } else {
//             buttons[i]->hide();
//         }
//     }
// }

// read in videos and thumbnails to this directory
std::vector<TheButtonInfo> getInfoIn (std::string loc) {

    // std::vector<TheButtonInfo> out =  std::vector<TheButtonInfo>();
    std::vector<TheButtonInfo> out;
    QDir dir(QString::fromStdString(loc) );
    QDirIterator it(dir);

    while (it.hasNext()) { // for all files

        QString f = it.next();

            if (f.contains("."))

#if defined(_WIN32)
            if (f.contains(".wmv"))  { // windows
#else
            if (f.contains(".mp4") || f.contains("MOV"))  { // mac/linux
#endif

            QString thumb = f.left( f .length() - 4) +".png";
            if (QFile(thumb).exists()) { // if a png thumbnail exists
                QImageReader *imageReader = new QImageReader(thumb);
                    QImage sprite = imageReader->read(); // read the thumbnail
                    if (!sprite.isNull()) {
                        QIcon* ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
                        QUrl* url = new QUrl(QUrl::fromLocalFile( f )); // convert the file location to a generic url
                        out . push_back(TheButtonInfo( url , ico  ) ); // add to the output list
                    }
                    else
                        qDebug() << "warning: skipping video because I couldn't process thumbnail " << thumb << endl;
            }
            else
                qDebug() << "warning: skipping video because I couldn't find thumbnail " << thumb << endl;
        }
    }

    return out;
}


int main(int argc, char *argv[]) {

    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR << endl;

    // create the Qt Application
    QApplication app(argc, argv);

    // collect all the videos in the folder
    std::vector<TheButtonInfo> videos;

    if (argc == 2)
        videos = getInfoIn( std::string(argv[1]) );

    if (videos.size() == 0) {

        const int result = QMessageBox::information(
                    NULL,
                    QString("Tomeo"),
                    QString("no videos found! Add command line argument to \"quoted\" file location."));
        exit(-1);
    }

    // the widget that will show the video
    QVideoWidget *videoWidget = new QVideoWidget;

    // the QMediaPlayer which controls the playback
    ThePlayer *player = new ThePlayer;
    player->setVideoOutput(videoWidget);

    // a row of buttons
    QWidget *buttonWidget = new QWidget();
    // a list of the buttons
    std::vector<TheButton*> buttons;

    // create the main window and layout
    QWidget window;
    QVBoxLayout *top = new QVBoxLayout();
    window.setLayout(top);
    window.setWindowTitle("tomeo");
    window.setMinimumSize(800, 680);

    // Create the ButtonNavigator
    ButtonNavigator* navigator = new ButtonNavigator();
    navigator->setButtons(&buttons);
    // Create a layout for the navigator
    QVBoxLayout* naviLayout = new QVBoxLayout();
    naviLayout->addWidget(navigator);
    // Add a new widget next to the buttonwidget for the navigator
    QWidget* navigatorWidget = new QWidget();
    navigatorWidget->setLayout(naviLayout);

    // Create the control buttons
    PreviousButton* previousButton = new PreviousButton(player, nullptr);
    NextButton* nextButton = new NextButton(player, nullptr);

    // Hide the button borders
    previousButton->setFlat(true);
    nextButton->setFlat(true);

    // Set icon size for previous and next buttons
    previousButton->setIconSize(QSize(30, 30)); // 设置图标大小
    nextButton->setIconSize(QSize(30, 30)); // 设置图标大小

    // Create a layout for the control buttons
    QHBoxLayout* controlLayout = new QHBoxLayout();
    controlLayout->addWidget(previousButton);
    controlLayout->addWidget(nextButton);

    // Add a new widget next to the buttonwidget for the control buttons
    QWidget* controlWidget = new QWidget();
    controlWidget->setLayout(controlLayout);

    // the buttons are arranged horizontally
    // QHBoxLayout *layout = new QHBoxLayout();
    // buttonWidget->setLayout(layout);

    // Create and add the pause button
    PauseToggleButton *pauseButton = new PauseToggleButton(&window);
    pauseButton->setFlat(true); // 隐藏按钮边框
    pauseButton->setIconSize(QSize(36, 36)); // 设置图标大小
    pauseButton->connect(pauseButton, &PauseToggleButton::toggled, player, &ThePlayer::handlePauseToggle);
   
    // Create and add the timeline slider
    TimelineSlider *timelineSlider = new TimelineSlider(&window);
    timelineSlider->setStyleSheet(
        "QSlider::groove:horizontal {"
        // "    border: 1px solid #000000;"
        "    height: 8px;"
        "    background: #ddd;"
        "    margin: 2px 0;"
        "}"
        "QSlider::handle:horizontal {"
        "    background: #00d0ff;"
        "    border: 5px solid #ffffff;"
        "    width: 12px;" // 设置按钮宽度
        "    height: 12px;" // 设置按钮高度
        "    margin: -2px 0;"
        "    border-radius: 6px;" // 设置按钮为圆形
        "}"
        "QSlider::sub-page:horizontal {"
        "    background: #00d0ff;"
        "    border: 1px solid #00d0ff;"
        "    height: 8px;"
        "    border-radius: 2px;"
        "}"
    );
    timelineSlider->connect(player, &QMediaPlayer::durationChanged, timelineSlider, &TimelineSlider::setDuration);
    timelineSlider->connect(player, &QMediaPlayer::positionChanged, timelineSlider, &TimelineSlider::setPosition);
    timelineSlider->connect(timelineSlider, &TimelineSlider::sliderMoved, player, &QMediaPlayer::setPosition);

    // Create and add the volume button
    // VolumeButton *volumeButton = new VolumeButton(&window);
    // volumeButton->connect(volumeButton, &VolumeButton::volumeChanged, player, &QMediaPlayer::setVolume);
    
    // Create and add the volume button
    QPushButton *volumeControlButton = new QPushButton(&window);
    volumeControlButton->setIcon(QIcon(":/icons/icons/volume_up_24dp_5F6368.svg"));
    volumeControlButton->setText("Volume");

    // Hide the button borders
    volumeControlButton->setFlat(true);
    volumeControlButton->setIconSize(QSize(36, 36)); // 设置图标大小

    VolumeButton *volumeSlider = new VolumeButton(&window);
    volumeSlider->setStyleSheet(
        "QSlider::groove:horizontal {"
        // "    border: 1px solid #000000;"
        "    height: 8px;"
        "    background: #ddd;"
        "    margin: 2px 0;"
        "}"
        "QSlider::handle:horizontal {"
        "    background: #00d0ff;"
        "    border: 5px solid #ffffff;"
        "    width: 12px;" // 设置按钮宽度
        "    height: 12px;" // 设置按钮高度
        "    margin: -2px 0;"
        "    border-radius: 6px;" // 设置按钮为圆形
        "}"
        "QSlider::sub-page:horizontal {"
        "    background: #00d0ff;"
        "    border: 1px solid #00d0ff;"
        "    height: 8px;"
        "    border-radius: 2px;"
        "}"
    );

    // Hide the volume slider by default
    volumeSlider->setVisible(false); 
    volumeSlider->setFixedWidth(200); // 限制音量条的宽度
    volumeSlider->connect(volumeSlider, &VolumeButton::volumeChanged, player, &QMediaPlayer::setVolume);

    // 布局管理器
    // QVBoxLayout *volumeLayout = new QVBoxLayout();
    // volumeLayout->addWidget(volumeControlButton);
    // volumeLayout->addWidget(volumeSlider);
    // QWidget *volumeWidget = new QWidget();
    // volumeWidget->setLayout(volumeLayout);

    // 连接按钮点击事件以显示/隐藏音量条
    QObject::connect(volumeControlButton, &QPushButton::clicked, [volumeSlider]() {
        volumeSlider->setVisible(!volumeSlider->isVisible());
    });

    // Create and add the replay button
    RestartButton *restartButton = new RestartButton(player, pauseButton, &window);
    restartButton->setFlat(true); // 隐藏按钮边框
    restartButton->setIconSize(QSize(36, 36)); // 设置图标大小

    // Create and add the fast forward button
    FastForwardButton *fastForwardButton = new FastForwardButton(player, pauseButton, &window);
    fastForwardButton->setFlat(true); // 隐藏按钮边框
    fastForwardButton->setIconSize(QSize(36, 36)); // 设置图标大小

    // Create and add the fast rewind button
    FastRewindButton *fastRewindButton = new FastRewindButton(player, &window);
    fastRewindButton->setFlat(true); // 隐藏按钮边框
    fastRewindButton->setIconSize(QSize(36, 36)); // 设置图标大小

    // Create and add the speed control button
    SpeedControlButton *speedControlButton = new SpeedControlButton(player, &window);
    speedControlButton->setFlat(true); // 隐藏按钮边框
    speedControlButton->setIconSize(QSize(36, 36)); // 设置图标大小


    // 将 volumeWidget 添加到主布局中
    // layout->addWidget(volumeWidget);
    
    // Create a layout for the pause button, replay button, fast forward button, and volume control
    QHBoxLayout *pauseVolumeLayout = new QHBoxLayout();
    
    // Add spacers to center the buttons
    QSpacerItem *leftSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *middleSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    pauseVolumeLayout->addItem(leftSpacer);
    pauseVolumeLayout->addWidget(restartButton); // 添加 replay 按钮到布局
    pauseVolumeLayout->addWidget(fastRewindButton); // 添加 fast rewind 按钮到布局
    pauseVolumeLayout->addWidget(pauseButton);
    pauseVolumeLayout->addWidget(fastForwardButton); // 添加 fast forward 按钮到布局
    pauseVolumeLayout->addWidget(volumeControlButton);
    pauseVolumeLayout->addWidget(volumeSlider);
    pauseVolumeLayout->addItem(middleSpacer);
    pauseVolumeLayout->addWidget(speedControlButton); // 添加 speed control 按钮到布局

    // Add the control row layout to a widget
    QWidget *pauseVolumeWidget = new QWidget();
    pauseVolumeWidget->setLayout(pauseVolumeLayout);

    // create the four buttons
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(previousButton);
    for ( int i = 0; i < 4; i++ ) {
        TheButton *button = new TheButton(buttonWidget);
        button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum); 
        button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo*))); // when clicked, tell the player to play.
        buttons.push_back(button);
        layout->addWidget(button);
        button->init(&videos.at(i));
    }
    layout->addWidget(nextButton);
    buttonWidget->setLayout(layout);

    // tell the player what buttons and videos are available
    player->setContent(&buttons, &videos);

    // add the video and the buttons to the top level widget
    top->addWidget(videoWidget);
    top->addWidget(timelineSlider);
    top->addWidget(controlWidget); // 添加控制行小部件
    top->addWidget(pauseVolumeWidget); // 添加暂停和音量控制小部件
    top->addWidget(buttonWidget);
    top->addWidget(navigatorWidget);
    top->addWidget(controlWidget);

    // // Connect the navigator signals to the rearrange function
    // QObject::connect(navigator, &ButtonNavigator::next, [&buttons, &videos]() {
    //     static int currentIndex = 0;
    //     currentIndex += 4;
    //     rearrangeButtons(buttons, videos, currentIndex, 4);
    // });

    // QObject::connect(navigator, &ButtonNavigator::previous, [&buttons, &videos]() {
    //     static int currentIndex = 0;
    //     currentIndex -= 4;
    //     rearrangeButtons(buttons, videos, currentIndex, 4);
    // });

    // Connect the previous button click event
    QObject::connect(previousButton, &QPushButton::clicked, [pauseButton, player]() {
        player->playPrevious();
        if (!pauseButton->isChecked()) {
            pauseButton->setIcon(QIcon(":/icons/icons/pause_24dp_5F6368.svg")); // 重置图标为播放图标
            pauseButton->setText("Pause"); // 重置文本为 "Play"
        }
    });

    // Connect the next button click event
    QObject::connect(nextButton, &QPushButton::clicked, [pauseButton, player]() {
        player->playNext();
        if (!pauseButton->isChecked()) {
            pauseButton->setIcon(QIcon(":/icons/icons/pause_24dp_5F6368.svg")); // 重置图标为播放图标
            pauseButton->setText("Pause"); // 重置文本为 "Play"
        }
    });



    // showtime!
    window.show();


    // wait for the app to terminate
    return app.exec();
}
