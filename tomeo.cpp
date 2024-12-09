//
//    ______
//   /_  __/___  ____ ___  ___  ____
//    / / / __ \/ __ `__ \/ _ \/ __ \
//   / / / /_/ / / / / / /  __/ /_/ /
//  /_/  \____/_/ /_/ /_/\___/\____/
//              video for sports enthusiasts...
//
//

#include <iostream>
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
    // Create a layout for the control buttons
    QHBoxLayout* controlLayout = new QHBoxLayout();
    controlLayout->addWidget(previousButton);
    controlLayout->addWidget(nextButton);
    // Add a new widget next to the buttonwidget for the control buttons
    QWidget* controlWidget = new QWidget();
    controlWidget->setLayout(controlLayout);

    // the buttons are arranged horizontally
    QHBoxLayout *layout = new QHBoxLayout();
    buttonWidget->setLayout(layout);


    // create the four buttons
    for ( int i = 0; i < 4; i++ ) {
        TheButton *button = new TheButton(buttonWidget);
        button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo*))); // when clicked, tell the player to play.
        buttons.push_back(button);
        layout->addWidget(button);
        button->init(&videos.at(i));
    }

    // tell the player what buttons and videos are available
    player->setContent(&buttons, &videos);

    // create the main window and layout
    QWidget window;
    QVBoxLayout *top = new QVBoxLayout();
    window.setLayout(top);
    window.setWindowTitle("tomeo");
    window.setMinimumSize(800, 680);

    // Create and add the pause button
    PauseToggleButton *pauseButton = new PauseToggleButton(&window);
    pauseButton->connect(pauseButton, &PauseToggleButton::toggled, player, &ThePlayer::handlePauseToggle);
   
    // Create and add the timeline slider
    TimelineSlider *timelineSlider = new TimelineSlider(&window);
    timelineSlider->connect(player, &QMediaPlayer::durationChanged, timelineSlider, &TimelineSlider::setDuration);
    timelineSlider->connect(player, &QMediaPlayer::positionChanged, timelineSlider, &TimelineSlider::setPosition);
    timelineSlider->connect(timelineSlider, &TimelineSlider::sliderMoved, player, &QMediaPlayer::setPosition);

    // Create and add the volume button
    VolumeButton *volumeButton = new VolumeButton(&window);
    volumeButton->connect(volumeButton, &VolumeButton::volumeChanged, player, &QMediaPlayer::setVolume);
    
    // add the video and the buttons to the top level widget
    top->addWidget(videoWidget);
    top->addWidget(timelineSlider);
    top->addWidget(pauseButton);
    top->addWidget(volumeButton);
    top->addWidget(buttonWidget);
    // top->addWidget(navigatorWidget);
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

    // showtime!
    window.show();


    // wait for the app to terminate
    return app.exec();
}
