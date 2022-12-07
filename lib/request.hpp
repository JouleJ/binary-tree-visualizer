#pragma once

#include <QString>

#include <cstdint>

namespace lib {
class ArgumentScheme {
  public:
    virtual ~ArgumentScheme() = default;

    virtual QString getName() const = 0;
    virtual bool validateValue(int64_t value) const = 0;
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

class AnimationDelayRequestScheme : public RequestScheme {
    class AnimationDelayArgumentScheme : public lib::ArgumentScheme {
    public:
        explicit AnimationDelayArgumentScheme() = default;

        QString getName() const override {
            return "Задержка (мс)";
        }

        bool validateValue(int64_t value) const override {
        return value > 0;
        }
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

bool validateRequest(const RequestScheme *requestScheme, const int64_t *values,
                     size_t valueCount);
} // namespace lib
