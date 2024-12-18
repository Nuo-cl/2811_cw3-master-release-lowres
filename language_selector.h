#ifndef LANGUAGE_SELECTOR_H
#define LANGUAGE_SELECTOR_H

#include <QObject>
#include <QPushButton>
#include <QApplication>
#include <QInputDialog>
#include <functional>

class LanguageSelector : public QObject {
    Q_OBJECT

public:
    LanguageSelector(QPushButton* languageButton, std::function<void(const QString&)> updateTextFunc);
    
private slots:
    void selectLanguage();

private:
    QPushButton* languageButton;
    std::function<void(const QString&)> updateTextFunc;
};

#endif // LANGUAGE_SELECTOR_H