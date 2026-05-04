#pragma once

#include "ethiopian.hpp"
#include "gregorian.hpp"

ethiopian::Date toEthiopian(const gregorian::Date &d)
{
    // algorithm assumes Ethiopian leap years are always leap years for Gregorian.
    // Which is not true for 1900 and 2100.
    // e.g., 2024 was a leap year in Gregorian calendar, which was 2016 in EC, also a leap year in that system.
    if(d.year < 1900 || d.year > 2100) {
        throw std::runtime_error("Not yet implemented");
    }

    gregorian::Date enkutatash(d.year, gregorian::Month::September, gregorian::yearIsLeapYear(d.year + 1) ? 12 : 11);
    int yearOffset = d >= enkutatash ? 7 : 8;
    if(d >= enkutatash) {
        return ethiopian::fromDayOfYear(d.year - yearOffset, 1 + (d - enkutatash));
    }
    gregorian::Date lastEnkutatash(d.year - 1, gregorian::Month::September, gregorian::yearIsLeapYear(d.year) ? 12 : 11);
    return ethiopian::fromDayOfYear(d.year - yearOffset, 1 + (d - lastEnkutatash));
}
