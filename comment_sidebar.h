#ifndef COMMENT_SIDEBAR_H
#define COMMENT_SIDEBAR_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QListWidget>
#include <QSqlDatabase>
#include <QDateTime>

class CommentSidebar : public QWidget {
    Q_OBJECT

public:
    explicit CommentSidebar(QWidget *parent = nullptr);
    void loadComments(const QString& videoPath);
    void clearComments();

public slots:
    void addNewComment();
    void show();

private:
    QVBoxLayout* layout;
    QListWidget* commentList;
    QTextEdit* commentInput;
    QPushButton* submitButton;
    QString currentVideoPath;
    bool hasBeenShown = false;

    void setupUI();
    void setupDatabase();
    void refreshComments();
};

#endif // COMMENT_SIDEBAR_H