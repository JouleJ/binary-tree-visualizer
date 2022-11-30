#include "gui/request-form.hpp"

#include <iostream>

RequestForm::RequestForm(QWidget *parent,
                         const lib::RequestScheme *_requestScheme)
    : QFrame(parent), requestScheme(_requestScheme) {
  setFrameShape(QFrame::Box);
  formLayout = new QFormLayout;

  const size_t argumentCount = requestScheme->getArgumentCount();
  argumentNameLabels.resize(argumentCount, nullptr);
  argumentValueLineEdits.resize(argumentCount, nullptr);
  for (size_t idx = 0; idx < argumentCount; ++idx) {
    argumentNameLabels[idx] =
        new QLabel(requestScheme->getArgumentScheme(idx)->getName(), this);
    argumentValueLineEdits[idx] = new QLineEdit(this);
    formLayout->addRow(argumentNameLabels[idx], argumentValueLineEdits[idx]);

    QObject::connect(argumentValueLineEdits[idx], &QLineEdit::textChanged, this,
                     &RequestForm::onArgumentValueChanged);
  }

  setLayout(formLayout);
}

void RequestForm::onArgumentValueChanged(const QString &_ignored) {
  static_cast<void>(_ignored);
  requestValues.clear();
  isRequestValid = false;

  const size_t argumentCount = requestScheme->getArgumentCount();
  for (size_t idx = 0; idx < argumentCount; ++idx) {
    const QString userInputString = argumentValueLineEdits[idx]->text();
    bool isCorrectInteger = false;
    const int64_t userInputI64 = userInputString.toLongLong(&isCorrectInteger);

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
    : dataStructure(_dataStructure) {
  setFrameShape(QFrame::Box);
  formLayout = new QFormLayout;

  const size_t requestSchemeCount = dataStructure->getRequestSchemeCount();
  requestButtons.resize(requestSchemeCount, nullptr);
  requestForms.resize(requestSchemeCount, nullptr);
  for (size_t idx = 0; idx < requestSchemeCount; ++idx) {
    const lib::RequestScheme *requestScheme =
        dataStructure->getRequestScheme(idx);
    requestButtons[idx] = new QPushButton(requestScheme->getName(), this);
    requestForms[idx] = new RequestForm(this, requestScheme);
    formLayout->addRow(requestButtons[idx], requestForms[idx]);

    QObject::connect(requestButtons[idx], &QPushButton::clicked,
                     requestForms[idx], &RequestForm::onAdjacentButtonClicked);
    QObject::connect(requestForms[idx], &RequestForm::userRequested, this,
                     &Requester::onUserRequested);
  }

  setLayout(formLayout);
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

    const size_t requestResult =
        dataStructure->executeRequest(requestScheme, firstValue);
    emit requestExecuted();

    if (requestScheme->doesReturn()) {
      std::cout << "requestResult = " << requestResult << "\n";
    }
  } else {
    std::cout << "Requester::onUserRequested(<INVALID REQUEST>)\n";
  }
}
