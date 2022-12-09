#pragma once

#include <cstdint>
#include <vector>

#include <QFrame>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

class IntArrayEdit : public QFrame {
    Q_OBJECT

  private:
    QVBoxLayout *outerLayout = nullptr;
    QHBoxLayout *innerLayout = nullptr;

    QLineEdit *arraySizeLineEdit = nullptr;
    QPushButton *finishButton = nullptr;
    std::vector<QLineEdit *> arrayElementLineEdit;

    int64_t getMaxArraySize() const;

  private slots:
    void onArraySizeChanged(const QString &newText);
    void onFinishButtonPressed();

  public:
    IntArrayEdit(QWidget *parent);

    std::vector<int64_t> getArray(bool &isOk) const;

  signals:
    void userFinished(std::vector<int64_t> array);
};
