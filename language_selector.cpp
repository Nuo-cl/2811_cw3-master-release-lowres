#include "language_selector.h"

LanguageSelector::LanguageSelector(QPushButton* languageButton, std::function<void(const QString&)> updateTextFunc)
    : languageButton(languageButton), updateTextFunc(updateTextFunc) {
    // 设置按钮图标
    languageButton->setIcon(QIcon(":icons/icons/language_24dp_5F6368.svg"));
    languageButton->setIconSize(QSize(30, 30));
    languageButton->setText("Language");
    languageButton->setFlat(true); // 隐藏按钮边框
    QObject::connect(languageButton, &QPushButton::clicked, this, &LanguageSelector::selectLanguage);
}

void LanguageSelector::selectLanguage() {
    QStringList languages;
    languages << "中文" << "English";
    bool ok;
    QString selectedLanguage = QInputDialog::getItem(nullptr, "Select Language", "Language:", languages, 0, false, &ok);
    if (ok && !selectedLanguage.isEmpty()) {
        updateTextFunc(selectedLanguage);
    }
}