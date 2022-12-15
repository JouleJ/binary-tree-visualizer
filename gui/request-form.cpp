#include "gui/request-form.hpp"

#include <iostream>

RequestForm::RequestForm(QWidget *parent,
                         const lib::RequestScheme *_requestScheme)
    : QFrame(parent), requestScheme(_requestScheme) {
    setFrameShape(QFrame::Box);
    formLayout = new QFormLayout;

    const size_t argumentCount = requestScheme->getArgumentCount();
    argumentNameLabels.resize(argumentCount, nullptr);
    argumentValueSpinBoxes.resize(argumentCount, nullptr);
    for (size_t idx = 0; idx < argumentCount; ++idx) {
        argumentNameLabels[idx] =
            new QLabel(requestScheme->getArgumentScheme(idx)->getName(), this);

        argumentValueSpinBoxes[idx] = new QSpinBox(this);
        argumentValueSpinBoxes[idx]->setMinimum(
            requestScheme->getArgumentScheme(idx)->getDefaultMinimum());
        argumentValueSpinBoxes[idx]->setMaximum(
            requestScheme->getArgumentScheme(idx)->getDefaultMaximum());
        argumentValueSpinBoxes[idx]->setSuffix(
            requestScheme->getArgumentScheme(idx)->getDefaultSuffix());
        argumentValueSpinBoxes[idx]->setPrefix(
            requestScheme->getArgumentScheme(idx)->getDefaultPrefix());
        argumentValueSpinBoxes[idx]->setValue(
            requestScheme->getArgumentScheme(idx)->getDefaultValue());
        argumentValueSpinBoxes[idx]->setSingleStep(
            requestScheme->getArgumentScheme(idx)->getDefaultSingleStep());

        formLayout->addRow(argumentNameLabels[idx],
                           argumentValueSpinBoxes[idx]);

        QObject::connect(argumentValueSpinBoxes[idx], &QSpinBox::textChanged,
                         this, &RequestForm::onArgumentValueChanged);
    }

    setLayout(formLayout);

    requestValues.resize(argumentCount, 0);
    isRequestValid = validateRequest(requestScheme, requestValues.data(), requestValues.size());
}

void RequestForm::onArgumentValueChanged(const QString &_ignored) {
    static_cast<void>(_ignored);
    requestValues.clear();
    isRequestValid = false;

    const size_t argumentCount = requestScheme->getArgumentCount();
    for (size_t idx = 0; idx < argumentCount; ++idx) {
        const QString userInputString =
            argumentValueSpinBoxes[idx]->cleanText();
        bool isCorrectInteger = false;
        const int64_t userInputI64 =
            userInputString.toLongLong(&isCorrectInteger);

        if (!isCorrectInteger) {
            requestValues.clear();
            return;
        }

        requestValues.push_back(userInputI64);
    }

    if (validateRequest(requestScheme, requestValues.data(),
                        requestValues.size())) {
        isRequestValid = true;
    } else {
        requestValues.clear();
    }
}

void RequestForm::onAdjacentButtonClicked() {
    emit userRequested(requestScheme, isRequestValid, requestValues.data(),
                       requestValues.size());
}

Requester::Requester(QWidget *parent, lib::DataStructure *_dataStructure)
    : QFrame(parent), dataStructure(_dataStructure) {
    setFrameShape(QFrame::Box);
    formLayout = new QFormLayout;

    const size_t requestSchemeCount = dataStructure->getRequestSchemeCount();
    requestButtons.resize(requestSchemeCount, nullptr);
    requestForms.resize(requestSchemeCount, nullptr);

    int maxButtonWidth = 0;
    int maxButtonHeight = 0;
    
    for (size_t idx = 0; idx < requestSchemeCount; ++idx) {
        const lib::RequestScheme *requestScheme =
            dataStructure->getRequestScheme(idx);
        requestButtons[idx] = new QPushButton(requestScheme->getName(), this);
        // [TODO - @GandarfHSE]: make button width calculation less tricky
        maxButtonWidth = std::max(maxButtonWidth, 10 * (int)requestScheme->getName().size());
        maxButtonHeight = std::max(maxButtonHeight, requestButtons[idx]->height());

        requestForms[idx] = new RequestForm(this, requestScheme);
        formLayout->addRow(requestButtons[idx], requestForms[idx]);

        QObject::connect(requestButtons[idx], &QPushButton::clicked,
                         requestForms[idx],
                         &RequestForm::onAdjacentButtonClicked);
        QObject::connect(requestForms[idx], &RequestForm::userRequested, this,
                         &Requester::onUserRequested);
    }
    
    QSize maxButtonSize = QSize(maxButtonWidth, maxButtonHeight);
    for (auto &b : requestButtons) {
        b->setMinimumSize(maxButtonSize);
    }

    requestResultStaticLabel = new QLabel("Результат запроса", this);
    requestResultDynamicLabel = new QLabel("Запросов пока не было", this);

    requestResultStaticLabel->setMinimumSize(maxButtonSize);
    requestResultStaticLabel->setAlignment(Qt::AlignCenter);

    requestResultDynamicLabel->setMinimumSize(maxButtonSize);
    requestResultDynamicLabel->setAlignment(Qt::AlignCenter);

    formLayout->addRow(requestResultStaticLabel, requestResultDynamicLabel);

    setLayout(formLayout);
}

void Requester::onRequestReturnedResult(int64_t result) {
    requestResultDynamicLabel->setText(QString::number(result));
}

void Requester::onInvalidRequest() {
    requestResultDynamicLabel->setText("Некорректный запрос");
}

void Requester::onUserRequested(const lib::RequestScheme *requestScheme,
                                bool isRequestValid, const int64_t *firstValue,
                                size_t valueCount) {
    if (isRequestValid) {
        std::cout << "Requester::onUserRequested(";
        for (size_t idx = 0; idx < valueCount; ++idx) {
            if (idx != 0) {
                std::cout << ", ";
            }
            std::cout << firstValue[idx];
        }
        std::cout << ")\n";

        const int requestResult =
            dataStructure->executeRequest(requestScheme, firstValue);
        emit requestExecuted();

        if (requestScheme->doesReturn()) {
            onRequestReturnedResult(requestResult);
        }
    } else {
        onInvalidRequest();
    }
}
