#pragma once

#include "lunar.hpp"

namespace gregorian {
    Date getPaschalFullMoon(int gregorianYear)
    {
        lunar::Year lunarYear(gregorianYear);
        Date ecclesiasticalEquinox(gregorianYear, Month::March, 21);
        Date d = ecclesiasticalEquinox;
        while(lunarYear.getPhase(d) != lunar::Phase::FullMoon) {
            d = d.addDays(1);
            assert(d.year == gregorianYear);
        }
        assert(d <= Date(gregorianYear, Month::April, 18));
        return d;
    }

    Date getEaster(int gregorianYear)
    {
        Date easter = getPaschalFullMoon(gregorianYear);
        // Easter is _always_ after, if the paschal full moon is on a Sunday, then Easter is next week
        easter = easter.addDays(1);
        while(easter.getDayOfWeek() != DayOfWeek::Sunday) {
            easter = easter.addDays(1);
        }
        assert(easter >= Date(gregorianYear, Month::March, 22));
        assert(easter <= Date(gregorianYear, Month::April, 25));
        return easter;
    }
}
