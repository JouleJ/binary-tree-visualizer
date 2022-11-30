#pragma once

#include "lib/data-structure.hpp"

#include <QFormLayout>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <vector>

class RequestForm : public QFrame {
  Q_OBJECT

private:
  const lib::RequestScheme *requestScheme;
  QFormLayout *formLayout;

  std::vector<QLabel *> argumentNameLabels;
  std::vector<QLineEdit *> argumentValueLineEdits;

  std::vector<int64_t> requestValues;
  bool isRequestValid = false;

private slots:
  void onArgumentValueChanged(const QString &_ignored);

public:
  explicit RequestForm(QWidget *parent,
                       const lib::RequestScheme *_requestScheme);

public slots:
  void onAdjacentButtonClicked();

signals:
  void userRequested(const lib::RequestScheme *requestScheme,
                     bool isRequestValid, const int64_t *firstValue,
                     size_t valueCount);
};

class Requester : public QFrame {
  Q_OBJECT

private:
  lib::DataStructure *dataStructure;
  QFormLayout *formLayout;

  std::vector<QPushButton *> requestButtons;
  std::vector<RequestForm *> requestForms;

private slots:
  void onUserRequested(const lib::RequestScheme *requestScheme,
                       bool isRequestValid, const int64_t *firstValue,
                       size_t valueCount);

public:
  explicit Requester(QWidget *parent, lib::DataStructure *_dataStructre);

signals:
  void requestExecuted();
};
