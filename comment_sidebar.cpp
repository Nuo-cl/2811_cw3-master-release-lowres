#include "comment_sidebar.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QListWidgetItem>

CommentSidebar::CommentSidebar(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupDatabase();
}

void CommentSidebar::setupUI() {
    setFixedWidth(300); // 设置侧边栏宽度
    setStyleSheet("background-color: white; border-left: 1px solid #ccc;");
    
    layout = new QVBoxLayout(this);
    
    // 评论列表
    commentList = new QListWidget(this);
    commentList->setStyleSheet(
        "QListWidget {"
        "   border: none;"
        "   background-color: white;"
        "}"
        "QListWidget::item {"
        "   padding: 10px;"
        "   border-bottom: 1px solid #eee;"
        "}"
    );
    
    // 评论输入框
    commentInput = new QTextEdit(this);
    commentInput->setPlaceholderText("输入评论...");
    commentInput->setMaximumHeight(100);
    
    // 提交按钮
    submitButton = new QPushButton("发表评论", this);
    submitButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #00d0ff;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px;"
        "   border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #00b8e6;"
        "}"
    );
    
    layout->addWidget(commentList, 1);
    layout->addWidget(commentInput);
    layout->addWidget(submitButton);
    
    connect(submitButton, &QPushButton::clicked, this, &CommentSidebar::addNewComment);
    
    hide(); // 初始隐藏侧边栏
}

void CommentSidebar::setupDatabase() {
    // 创建一个独立的数据库连接，避免与 ThePlayer 的连接冲突
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "CommentConnection"); // 指定连接名称
    db.setDatabaseName("comments.db"); // 使用不同的数据库文件
    
    if (!db.open()) {
        qDebug() << "无法打开评论数据库:" << db.lastError().text();
        return;
    }

    QSqlQuery query(db);
    // 创建表格，如果不存在的话
    QString createTable = "CREATE TABLE IF NOT EXISTS Comments ("
                          "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "video_path TEXT,"
                          "content TEXT,"
                          "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP"
                          ")";
    if (!query.exec(createTable)) {
        qDebug() << "创建评论表格失败:" << query.lastError().text();
    }
}

void CommentSidebar::loadComments(const QString& videoPath) {
    currentVideoPath = videoPath;
    refreshComments();
}

void CommentSidebar::refreshComments() {
    commentList->clear();
    
    // 使用独立的数据库连接
    QSqlDatabase db = QSqlDatabase::database("CommentConnection");
    if (!db.isOpen()) {
        qDebug() << "评论数据库未打开";
        return;
    }

    QSqlQuery query(db);
    query.prepare("SELECT content, timestamp FROM Comments WHERE video_path = ? ORDER BY timestamp DESC");
    query.addBindValue(currentVideoPath);
    
    if (query.exec()) {
        while (query.next()) {
            QString content = query.value(0).toString();
            QDateTime timestamp = query.value(1).toDateTime();
            
            QString displayText = QString("%1\n%2")
                .arg(timestamp.toString("yyyy-MM-dd hh:mm:ss"))
                .arg(content);
                
            QListWidgetItem* item = new QListWidgetItem(displayText);
            commentList->addItem(item);
        }
    }
}

void CommentSidebar::addNewComment() {
    QString content = commentInput->toPlainText().trimmed();
    if (content.isEmpty() || currentVideoPath.isEmpty()) {
        return;
    }
    
    // 使用独立的数据库连接
    QSqlDatabase db = QSqlDatabase::database("CommentConnection");
    if (!db.isOpen()) {
        qDebug() << "评论数据库未打开";
        return;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO Comments (video_path, content) VALUES (?, ?)");
    query.addBindValue(currentVideoPath);
    query.addBindValue(content);
    
    if (query.exec()) {
        commentInput->clear();
        refreshComments();
    } else {
        qDebug() << "添加评论失败:" << query.lastError().text();
    }
}

void CommentSidebar::show() {
    QWidget::show();
    if (!hasBeenShown) {
        refreshComments();
        hasBeenShown = true;
    }
}

void CommentSidebar::clearComments() {
    commentList->clear();
    commentInput->clear();
    currentVideoPath.clear();
}