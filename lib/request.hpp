#pragma once

#include "lib/common.hpp"

#include <QString>

#include <cstdint>

namespace lib {
class ArgumentScheme {
  public:
    virtual ~ArgumentScheme() = default;

    virtual QString getName() const = 0;
    virtual bool validateValue(int64_t value) const = 0;

    virtual int getDefaultValue() const { return 0; }
    virtual QString getDefaultSuffix() const { return QString(); }
    virtual QString getDefaultPrefix() const { return QString(); }
    virtual int getDefaultMinimum() const { return 0; }
    virtual int getDefaultMaximum() const { return 100; }
    virtual int getDefaultSingleStep() const { return 1; }
};

class RequestScheme {
  public:
    virtual ~RequestScheme() = default;

    virtual QString getName() const = 0;
    virtual bool postValidateValues(const int64_t *values) const = 0;

    virtual size_t getArgumentCount() const = 0;
    virtual const ArgumentScheme *getArgumentScheme(size_t idx) const = 0;

    virtual bool doesReturn() const = 0;
};

bool validateRequest(const RequestScheme *requestScheme, const int64_t *values,
                     size_t valueCount);

// Common RequestSchemes
class AnimationDelayRequestScheme : public RequestScheme {
    class AnimationDelayArgumentScheme : public lib::ArgumentScheme {
      public:
        explicit AnimationDelayArgumentScheme() = default;

        QString getName() const override { return "Задержка (мс)"; }

        bool validateValue(int64_t value) const override { return value > 0; }

        int getDefaultMaximum() const override { return 9999; }

        int getDefaultMinimum() const override { return 200; }

        int getDefaultValue() const override {
            return common::defaultAnimationDelay;
        }

        int getDefaultSingleStep() const override { return 100; }

        QString getDefaultSuffix() const override { return QString{" ms"}; }
    };

    AnimationDelayArgumentScheme arg;

  public:
    explicit AnimationDelayRequestScheme() = default;

    QString getName() const override { return "Задержка анимации"; }

    bool postValidateValues(const int64_t *values) const override {
        static_cast<void>(values);
        return true;
    }

    size_t getArgumentCount() const override { return 1; }

    const ArgumentScheme *getArgumentScheme(size_t idx) const override {
        switch (idx) {
        case 0:
            return &arg;
        default:
            throw std::runtime_error(
                "AnimationDelayRequestScheme::getArgumentScheme: idx > 0");
        }
    }

    bool doesReturn() const override { return false; }
};

class SizeUnitRequestScheme : public RequestScheme {
    class SizeUnitArgumentScheme : public lib::ArgumentScheme {
      public:
        explicit SizeUnitArgumentScheme() = default;

        QString getName() const override { return "Размер юнита:"; }

        bool validateValue(int64_t value) const override { return value > 0; }

        int getDefaultMaximum() const override { return 40; }

        int getDefaultMinimum() const override { return 8; }

        int getDefaultValue() const override { return common::defaultSizeUnit; }

        int getDefaultSingleStep() const override { return 2; }

        QString getDefaultSuffix() const override { return QString{" px"}; }
    };

    SizeUnitArgumentScheme arg;

  public:
    explicit SizeUnitRequestScheme() = default;

    QString getName() const override { return "Выбрать размер юнита"; }

    bool postValidateValues(const int64_t *values) const override {
        static_cast<void>(values);
        return true;
    }

    size_t getArgumentCount() const override { return 1; }

    const ArgumentScheme *getArgumentScheme(size_t idx) const override {
        switch (idx) {
        case 0:
            return &arg;
        default:
            throw std::runtime_error(
                "AnimationDelayRequestScheme::getArgumentScheme: idx > 0");
        }
    }

    bool doesReturn() const override { return false; }
};

} // namespace lib
