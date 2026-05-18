#pragma once
/*
 * not to be confused with standard Julian days, astronimcal math have their own special julian concept
 * Using the definition from Astronomical Algorithms, 2nd ed. by Jean Meeus
 */

#include "conversion.hpp"

namespace astrojulian {
    // the given formula I use seems calibrated for Gregorian dates?
    // TODO: would like to confirm it, but the math does seem to be correct for gregorian and not julian
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

    constexpr double getDeltaT(int year)
    {
        if(year < 2000) {
            throw std::runtime_error("not yet implemented");
        }
        double t = (year-2000.0)/100.0;
        double result = 102 + 102 * t + 25.3 * t * t;
        if(year >= 2000 && year < 2100) {
            result += .37 * (year - 2100);
        }
        return result;
    }
    /*
     * should it actually be given a date, and then using the year for getDeltaT?
     constexpr double getDynamicalTime(int year)
     {
     return getDeltaT(year) + getJulianDay(year);
     }
     */
}
