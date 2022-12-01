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

bool validateRequest(const RequestScheme *requestScheme, const int64_t *values,
                     size_t valueCount);
} // namespace lib
