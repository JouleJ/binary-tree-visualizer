#include "gui/request-form.hpp"

RequestForm::RequestForm(QWidget *parent,
                         const lib::RequestScheme *_requestScheme)
    : QFrame(parent), requestScheme(_requestScheme),
      requestValues(requestScheme->getArgumentCount(), 0) {
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
}

void RequestForm::onArgumentValueChanged(const QString &_ignored) {
    static_cast<void>(_ignored);

    const size_t argumentCount = requestScheme->getArgumentCount();
    for (size_t idx = 0; idx < argumentCount; ++idx) {
        const QString userInputString =
            argumentValueSpinBoxes[idx]->cleanText();
        bool isCorrectInteger = false;
        const int64_t userInputI64 =
            userInputString.toLongLong(&isCorrectInteger);

        if (isCorrectInteger) {
            requestValues[idx] = userInputI64;
        } else {
            requestValues[idx] = {};
        }
    }
}

void RequestForm::onAdjacentButtonClicked() {
    const size_t n = requestValues.size();
    std::vector<int64_t> unwrapedValues;
    unwrapedValues.reserve(n);

    for (size_t i = 0; i < n; ++i) {
        if (requestValues[i]) {
            unwrapedValues.push_back(*requestValues[i]);
        } else {
            emit userRequested(requestScheme, false, nullptr, 0);
            return;
        }
    }

    emit userRequested(requestScheme,
                       validateRequest(requestScheme, unwrapedValues.data(),
                                       unwrapedValues.size()),
                       unwrapedValues.data(), unwrapedValues.size());
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
        maxButtonWidth =
            std::max(maxButtonWidth, 10 * (int)requestScheme->getName().size());
        maxButtonHeight =
            std::max(maxButtonHeight, requestButtons[idx]->height());

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
    static_cast<void>(valueCount); // UNUSED

    if (isRequestValid) {
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
