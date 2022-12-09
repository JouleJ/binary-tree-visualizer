#include "int-array-edit.hpp"

IntArrayEdit::IntArrayEdit(QWidget *parent) : QFrame(parent) {
    outerLayout = new QVBoxLayout;

    arraySizeLineEdit = new QLineEdit(this);
    outerLayout->addWidget(arraySizeLineEdit);

    innerLayout = new QHBoxLayout;
    outerLayout->addLayout(innerLayout);

    finishButton = new QPushButton("Готово", this);
    outerLayout->addWidget(finishButton);

    setLayout(outerLayout);

    QObject::connect(arraySizeLineEdit, &QLineEdit::textChanged, this,
                     &IntArrayEdit::onArraySizeChanged);
    QObject::connect(finishButton, &QPushButton::clicked, this,
                     &IntArrayEdit::onFinishButtonPressed);
}

int64_t IntArrayEdit::getMaxArraySize() const { return 16; }

void IntArrayEdit::onFinishButtonPressed() {
    bool isOk = false;
    const std::vector<int64_t> array = getArray(isOk);

    if (isOk) {
        emit userFinished(array);
    }
}

void IntArrayEdit::onArraySizeChanged(const QString &newText) {
    bool isNumber = false;
    int64_t number = newText.toLongLong(&isNumber);
    if (!isNumber || number > getMaxArraySize() || number < 1) {
        return;
    }

    for (QLineEdit *widget : arrayElementLineEdit) {
        innerLayout->removeWidget(widget);
        widget->deleteLater();
    }

    arrayElementLineEdit.clear();
    for (int64_t i = 0; i < number; ++i) {
        arrayElementLineEdit.push_back(new QLineEdit(this));
        innerLayout->addWidget(arrayElementLineEdit.back());
    }
}

std::vector<int64_t> IntArrayEdit::getArray(bool &isOk) const {
    std::vector<int64_t> input;
    isOk = true;

    const size_t n = arrayElementLineEdit.size();
    if (n < 1) {
        isOk = false;
        return {};
    }

    input.reserve(n);

    for (size_t i = 0; i < n; ++i) {
        QString text = arrayElementLineEdit[i]->text();
        bool isNumber = false;
        int64_t number = text.toLongLong(&isNumber);
        if (!isNumber) {
            isOk = false;
            return {};
        }

        input.push_back(number);
    }

    return input;
}
