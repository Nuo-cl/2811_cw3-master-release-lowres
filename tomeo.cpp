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
#include "language_selector.h"
#include <QKeyEvent>
#include "fullscreen_event_filter.h"
#include "comment_sidebar.h"

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

void updateText(QPushButton* previousButton, QPushButton* nextButton, 
                PauseToggleButton* pauseButton, QPushButton* languageButton,
                QPushButton* fastRewindButton, QPushButton* fastForwardButton,
                QPushButton* restartButton, QPushButton* speedControlButton,
                QPushButton* volumeControlButton, QPushButton* fullscreenButton,
                QPushButton* likeButton, QPushButton* favoriteButton, QPushButton* commentButton,
                const QString& language) {
    if (language == "中文" || language == "语言") {
        previousButton->setText("上一个视频");
        nextButton->setText("下一个视频");
        pauseButton->setText(pauseButton->Paused() ? "恢复" : "暂停");
        languageButton->setText("语言");
        fastRewindButton->setText("快退");
        fastForwardButton->setText("快进");
        restartButton->setText("重播");
        speedControlButton->setText("倍速");
        volumeControlButton->setText("音量");
        fullscreenButton->setText("全屏");
        likeButton->setText("点赞 👍");
        favoriteButton->setText("收藏 ⭐️");
        commentButton->setText("评论");
    } else {
        previousButton->setText("Previous");
        nextButton->setText("Next");
        pauseButton->setText(pauseButton->Paused() ? "Resume" : "Pause");
        languageButton->setText("Select Language");
        fastRewindButton->setText("Rewind");
        fastForwardButton->setText("Fast Forward");
        restartButton->setText("Replay");
        speedControlButton->setText("Speed");
        volumeControlButton->setText("Volume");
        fullscreenButton->setText("Fullscreen");
        likeButton->setText("Like 👍");
        favoriteButton->setText("Favorite ⭐️");
        commentButton->setText("Comment");
    }
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

    // 定义全屏状态变量
    bool isVideoFullscreen = false;

    // 添加全屏按钮
    QPushButton* fullscreenButton = new QPushButton(&window);
    fullscreenButton->setIcon(QIcon(":/icons/icons/fullscreen.svg")); // 设置图标
    fullscreenButton->setText("全屏");
    fullscreenButton->setFlat(true); // 隐藏按钮边框
    fullscreenButton->setIconSize(QSize(36, 36)); // 设置图标大小

    // 将全屏按钮添加到暂停和音量控制的布局中
    pauseVolumeLayout->addWidget(fullscreenButton);

    QPushButton* languageButton = new QPushButton();

    // 连接全屏按钮的点击信号到槽函数
    QObject::connect(fullscreenButton, &QPushButton::clicked, [&isVideoFullscreen, &window, &top, &videoWidget, &timelineSlider, &controlWidget, &pauseVolumeWidget, &buttonWidget, &navigatorWidget, &languageButton, &fullscreenButton]() {
        if (!isVideoFullscreen) {
            // 隐藏其他控件
            timelineSlider->setVisible(false);
            controlWidget->setVisible(false);
            pauseVolumeWidget->setVisible(false);
            buttonWidget->setVisible(false);
            navigatorWidget->setVisible(false);
            languageButton->setVisible(false);
            
            // 让 videoWidget 占满整个窗口
            top->removeWidget(videoWidget);
            videoWidget->setParent(&window);
            top->addWidget(videoWidget, 1); // 设置���缩因子为1，使其占满空间
            
            // 修改按钮文本为“退出全屏”
            fullscreenButton->setText("退出全屏");
            
            isVideoFullscreen = true;
        } else {
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
            
            isVideoFullscreen = false;
        }
    });

    // 安装事件过滤器以捕捉 Esc 键
    FullscreenEventFilter* filter = new FullscreenEventFilter(&isVideoFullscreen, &window, top, videoWidget, 
                                                              timelineSlider, controlWidget, pauseVolumeWidget, 
                                                              buttonWidget, navigatorWidget, languageButton, 
                                                              fullscreenButton);
    window.installEventFilter(filter);

    // 创建“点赞”和“收藏”按钮
    QPushButton* likeButton = new QPushButton("点赞 👍", &window);
    // likeButton->setIcon(QIcon(":/icons/icons/like.svg")); // 移除图标设置
    // likeButton->setIconSize(QSize(36, 36)); // 移除图标大小设置

    QPushButton* favoriteButton = new QPushButton("收藏 ⭐️", &window);
    // favoriteButton->setIcon(QIcon(":/icons/icons/favorite.svg")); // 移除图标设置
    // favoriteButton->setIconSize(QSize(36, 36)); // 移除图标大小设置

    // 创建一个布局来放置“点赞”和“收藏”按钮，并对齐到右上方
    QHBoxLayout* favoriteLayout = new QHBoxLayout();
    favoriteLayout->setAlignment(Qt::AlignRight | Qt::AlignTop);
    favoriteLayout->addWidget(likeButton);
    favoriteLayout->addWidget(favoriteButton);

    // 将布局添加到视频上方
    top->insertLayout(0, favoriteLayout); // 在最顶部插入

    // 连接 ThePlayer 的信号到槽，以更新按钮的背景颜色
    QObject::connect(player, &ThePlayer::likeStatusChanged, [likeButton](bool liked) {
        if (liked) {
            likeButton->setStyleSheet("background-color: red;");
        } else {
            likeButton->setStyleSheet("");
        }
    });

    QObject::connect(player, &ThePlayer::favoriteStatusChanged, [favoriteButton](bool favorited) {
        if (favorited) {
            favoriteButton->setStyleSheet("background-color: yellow;");
        } else {
            favoriteButton->setStyleSheet("");
        }
    });

    CommentSidebar* commentSidebar = new CommentSidebar(&window);
    commentSidebar->hide();

    QPushButton* commentButton = new QPushButton("评论", &window);
    commentButton->setIcon(QIcon(":/icons/icons/comment_24dp_5F6368.svg")); 
    commentButton->setFlat(true);
    commentButton->setIconSize(QSize(36, 36));

    // 将评论按钮添加到现有布局中
    pauseVolumeLayout->addWidget(commentButton);

    // 连接信号和槽
    QObject::connect(commentButton, &QPushButton::clicked, [commentSidebar]() {
        if (commentSidebar->isVisible()) {
            commentSidebar->hide();
        } else {
            commentSidebar->show();
        }
    });

    QObject::connect(player, &ThePlayer::videoPathChanged, commentSidebar, &CommentSidebar::loadComments);

    // 创建一个主布局，并添加 top 和 commentSidebar
    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->addLayout(top);
    mainLayout->addWidget(commentSidebar);
    window.setLayout(mainLayout);

    // Create the language selector
    LanguageSelector languageSelector(languageButton, std::bind(updateText, previousButton, nextButton,
                                                                pauseButton, languageButton, fastRewindButton,
                                                                fastForwardButton, restartButton,
                                                                speedControlButton, volumeControlButton,
                                                                fullscreenButton, likeButton, favoriteButton,
                                                                commentButton,
                                                                std::placeholders::_1));

    updateText(previousButton, nextButton, 
            pauseButton, languageButton,
            fastRewindButton, fastForwardButton,
            restartButton, speedControlButton, volumeControlButton,
            fullscreenButton, likeButton, favoriteButton, commentButton,
            "English");

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

    QObject::connect(pauseButton, &PauseToggleButton::toggled, [&]() {
        QString currentLanguage = languageButton->text();
        updateText(previousButton, nextButton, pauseButton,
            languageButton, fastRewindButton,
            fastForwardButton, restartButton,
            speedControlButton, volumeControlButton,
            fullscreenButton, likeButton, favoriteButton, commentButton,
            currentLanguage);
    });
    

    // 连接按钮信号到槽，由 ThePlayer 处理点赞和收藏逻辑
    QObject::connect(likeButton, &QPushButton::clicked, player, &ThePlayer::likeCurrentVideo);
    QObject::connect(favoriteButton, &QPushButton::clicked, player, &ThePlayer::favoriteCurrentVideo);

    // add the video and the buttons to the top level widget
    top->addWidget(videoWidget);
    top->addWidget(timelineSlider);
    top->addWidget(controlWidget); // 添加控制行小部件
    top->addWidget(pauseVolumeWidget); // 添加暂停和音量控制小部件
    top->addWidget(buttonWidget);
    top->addWidget(navigatorWidget);
    top->addWidget(languageButton);
    top->addWidget(controlWidget);

        // showtime!
    window.show();

    // wait for the app to terminate
    return app.exec();
}
