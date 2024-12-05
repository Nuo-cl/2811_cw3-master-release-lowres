#include "button_navigator.h"
#include <QHBoxLayout>

ButtonNavigator::ButtonNavigator(QWidget *parent) : QWidget(parent), currentIndex(0), buttonsPerPage(4) {
    nextButton = new QPushButton("Next Preview", this);
    backButton = new QPushButton("Last Preview", this);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(backButton);
    layout->addWidget(nextButton);

    connect(nextButton, &QPushButton::clicked, this, &ButtonNavigator::showNext);
    connect(backButton, &QPushButton::clicked, this, &ButtonNavigator::showPrevious);

    setLayout(layout);
}

void ButtonNavigator::setButtons(std::vector<TheButton*>* buttons) {
    this->buttons = buttons;
    updateButtons();
}

void ButtonNavigator::showNext() {
    if (currentIndex + buttonsPerPage < buttons->size()) {
        currentIndex += buttonsPerPage;
        emit next();
        updateButtons();
    }
}

void ButtonNavigator::showPrevious() {
    if (currentIndex - buttonsPerPage >= 0) {
        currentIndex -= buttonsPerPage;
        emit previous();
        updateButtons();
    }
}

void ButtonNavigator::updateButtons() {
    for (int i = 0; i < buttons->size(); ++i) {
        if (i >= currentIndex && i < currentIndex + buttonsPerPage) {
            buttons->at(i)->show();
        } else {
            buttons->at(i)->hide();
        }
    }
}