#ifndef BUTTON_NAVIGATOR_H
#define BUTTON_NAVIGATOR_H

#include <QWidget>
#include <QPushButton>
#include <vector>
#include "the_button.h"

class ButtonNavigator : public QWidget {
    Q_OBJECT

public:
    ButtonNavigator(QWidget *parent = nullptr);

    void setButtons(std::vector<TheButton*>* buttons);

signals:
    void next();
    void previous();

private slots:
    void showNext();
    void showPrevious();

private:
    QPushButton* nextButton;
    QPushButton* backButton;
    std::vector<TheButton*>* buttons;
    int currentIndex;
    int buttonsPerPage;

    void updateButtons();
};

#endif // BUTTON_NAVIGATOR_H