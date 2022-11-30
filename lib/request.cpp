#include "lib/request.hpp"

namespace lib {
    bool validateRequest(const RequestScheme* requestScheme, const int64_t* values, size_t valueCount) {
        const size_t argumentCount = requestScheme->getArgumentCount();
        if (valueCount != argumentCount) {
            return false;
        }

        for (size_t idx = 0; idx < argumentCount; ++idx) {
            const ArgumentScheme *argumentScheme = requestScheme->getArgumentScheme(idx);
            if (!argumentScheme->validateValue(values[idx])) {
                return false;
            }
        }

        return requestScheme->postValidateValues(values);
    }
}
