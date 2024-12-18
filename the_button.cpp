//
//
//

#include "the_button.h"


void TheButton::init(TheButtonInfo* i) {
    setIcon( *(i->icon) );
    info =  i;
}


// void TheButton::init(TheButtonInfo* i) {
//     info = i;
//     setIcon(QIcon(*i->icon));
//     setIconSize(QSize(100, 100)); // 设置图标大小

//     // 设置视频控件的媒体内容
//     player->setMedia(*i->url);
//     player->play();
// }

void TheButton::clicked() {
    emit jumpTo(info); // tell the player to play this video
}
