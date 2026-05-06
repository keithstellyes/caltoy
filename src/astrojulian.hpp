/*
 * not to be confused with standard Julian days, astronimcal math have their own special julian concept
 * Using the definition from Astronomical Algorithms, 2nd ed. by Jean Meeus
 */

#include "gregorian.hpp"

namespace astrojulian {
    constexpr double getJulianDay(const gregorian::Date &d)
    {
        int a = d.year / 100;
        int b = 2 - a + a/4;
        int m = static_cast<int>(d.month) + 1;
        int y = d.year;
        if(m == 1 || m == 2) {
            y -= 1;
            m += 12;
        }
        return static_cast<double>(static_cast<int>(365.25*(y + 4716))) + static_cast<double>(static_cast<int>(30.6001*(m + 1))) +
            d.day + b - 1524.5;
    }
}
