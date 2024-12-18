// 确保连接信号和槽正确
QObject::connect(player, &ThePlayer::videoPathChanged, commentSidebar, &CommentSidebar::loadComments); 