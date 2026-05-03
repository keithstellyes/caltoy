#pragma once

#include "lunar.hpp"

namespace gregorian {
    class Easter {
        public:
            constexpr Easter(int gregorianYear)
            {
                lunar::Year lunarYear(gregorianYear);
            }
    }
}
