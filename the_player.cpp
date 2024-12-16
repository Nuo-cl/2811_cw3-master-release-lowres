#include "the_player.h"

// all buttons have been setup, store pointers here
// void ThePlayer::setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i) {
//     buttons = b;
//     infos = i;
//     jumpTo(buttons -> at(0) -> info);
// }
void ThePlayer::setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i) {
    buttons = b;
    infos = i;
    currentIndex = 0; // Initialize current index
    jumpTo(&infos->at(currentIndex));
}

// change the image and video for one button every one second
void ThePlayer::shuffle() {
    TheButtonInfo* i = & infos -> at (rand() % infos->size() );
//        setMedia(*i->url);
    buttons -> at( updateCount++ % buttons->size() ) -> init( i );
}

void ThePlayer::playStateChanged (QMediaPlayer::State ms) {
    switch (ms) {
        case QMediaPlayer::State::StoppedState:
            play(); // starting playing again...
            break;
    default:
        break;
    }
}

void ThePlayer::jumpTo (TheButtonInfo* button) {
    setMedia( * button -> url);
    play();
}

// void ThePlayer::playNext() {
//     currentIndex = (currentIndex + 1) % buttons -> size();
//     jumpTo(buttons -> at(currentIndex) -> info);
// }

// void ThePlayer::playPrevious() {
//     currentIndex = (currentIndex - 1 + buttons -> size()) % buttons -> size();
//     jumpTo(buttons -> at(currentIndex) -> info);
// }
void ThePlayer::playNext() {
    currentIndex = (currentIndex + 1) % infos->size();
    jumpTo(&infos->at(currentIndex));
}

void ThePlayer::playPrevious() {
    currentIndex = (currentIndex - 1 + infos->size()) % infos->size();
    jumpTo(&infos->at(currentIndex));
}

void ThePlayer::handlePauseToggle(bool paused) {
    if (paused) {
        pause();
    } else {
        play();
    }
}