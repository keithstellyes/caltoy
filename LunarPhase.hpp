#pragma once

#include <cstdint>

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>
#define LUNATION_LENGTH_DAYS 29.53

enum class LunarPhase {
    NewMoon,
    WaxingCrescent,
    FirstQuarter,
    WaxingGibbous,
    FullMoon,
    WaningGibbous,
    LastQuarter,
    WaningCrescent
};

#define UNIQUE_PHASE_COUNT 8

constexpr std::string_view to_string(LunarPhase phase)
{
    switch (phase)
    {
        case LunarPhase::NewMoon:         return "New Moon";
        case LunarPhase::WaxingCrescent:  return "Waxing Crescent";
        case LunarPhase::FirstQuarter:    return "First Quarter";
        case LunarPhase::WaxingGibbous:   return "Waxing Gibbous";
        case LunarPhase::FullMoon:        return "Full Moon";
        case LunarPhase::WaningGibbous:   return "Waning Gibbous";
        case LunarPhase::LastQuarter:     return "Last Quarter";
        case LunarPhase::WaningCrescent:  return "Waning Crescent";
    }

    return "Unknown"; // fallback for invalid values
}

constexpr char32_t unicodeCodepoint(LunarPhase phase)
{
    return 127761 + static_cast<int>(phase);
}
std::string to_utf8(char32_t codepoint)
{
    std::string out;

    if (codepoint <= 0x7F)
    {
        out.push_back(static_cast<char>(codepoint));
    }
    else if (codepoint <= 0x7FF)
    {
        out.push_back(static_cast<char>(0xC0 | (codepoint >> 6)));
        out.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
    }
    else if (codepoint <= 0xFFFF)
    {
        out.push_back(static_cast<char>(0xE0 | (codepoint >> 12)));
        out.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
    }
    else
    {
        out.push_back(static_cast<char>(0xF0 | (codepoint >> 18)));
        out.push_back(static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
    }

    return out;
}

inline std::ostream& operator<<(std::ostream& os, LunarPhase phase)
{
    return os << to_string(phase);
}

enum class GregorianMonth {
    January,
    February,
    March,
    April,
    May,
    June,
    July,
    August,
    September,
    October,
    November,
    December
};

constexpr std::string_view to_string(GregorianMonth month)
{
    switch (month)
    {
        case GregorianMonth::January:   return "January";
        case GregorianMonth::February:  return "February";
        case GregorianMonth::March:     return "March";
        case GregorianMonth::April:     return "April";
        case GregorianMonth::May:       return "May";
        case GregorianMonth::June:      return "June";
        case GregorianMonth::July:      return "July";
        case GregorianMonth::August:    return "August";
        case GregorianMonth::September: return "September";
        case GregorianMonth::October:   return "October";
        case GregorianMonth::November:  return "November";
        case GregorianMonth::December:  return "December";
    }

    return "Unknown";
}
inline std::ostream& operator<<(std::ostream& os, GregorianMonth month)
{
    return os << to_string(month);
}

constexpr uint8_t daysInGregorianMonth(GregorianMonth month, bool isLeapYear)
{
    switch(month)
    {
        case GregorianMonth::January:
            return 31;
        case GregorianMonth::February:
            return 28 + (isLeapYear ? 1 : 0);
        case GregorianMonth::March:
            return 31;
        case GregorianMonth::April:
            return 30;
        case GregorianMonth::May:
            return 31;
        case GregorianMonth::June:
            return 30;
        case GregorianMonth::July:
            return 31;
        case GregorianMonth::August:
            return 31;
        case GregorianMonth::September:
            return 30;
        case GregorianMonth::October:
            return 31;
        case GregorianMonth::November:
            return 30;
        case GregorianMonth::December:
            return 31;
        default:
            return -1;
    }
}

constexpr bool yearIsLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

enum class GregorianDayOfWeek {
    Sunday,
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday
};

class GregorianDate
{
    public:
        int year;
        GregorianMonth month;
        uint8_t day;
        constexpr GregorianDate() : year(2026), month(GregorianMonth::January), day(1) {}
        constexpr GregorianDate(int year, GregorianMonth month, uint8_t day) : year(year), month(month), day(day) {}
        constexpr bool isLeapYear() const
        {
            return yearIsLeapYear(year);
        }

        // naive algorithm
        constexpr GregorianDate addDays(int days) const
        {
            if(days < 0) {
                throw std::runtime_error("Not yet implemented");
            }
            int newDayInMonth = days + this->day;
            if(newDayInMonth <= daysInGregorianMonth(month, isLeapYear())) {
                return GregorianDate(year, month, newDayInMonth);
            }
            GregorianMonth nextMonth = static_cast<GregorianMonth>((static_cast<int>(month) + 1) % 12);
            int nextYear = year + (nextMonth == GregorianMonth::January ? 1 : 0);
            int daysToReachEndOfMonth = daysInGregorianMonth(month, isLeapYear()) - this->day + 1;
            return GregorianDate(nextYear, nextMonth, 1).addDays(days - daysToReachEndOfMonth);
        }

        // Sakamoto algorithm? https://stackoverflow.com/a/905468
        constexpr GregorianDayOfWeek getDayOfWeek() const
        {
            int m = static_cast<int>(month) + 1;
            int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
            int y = year;
            y -= m < 3;
            return static_cast<GregorianDayOfWeek>((y + y/4 - y/100 + y/400 + t[m-1] + day) % 7);
        }
};


class LunarYear
{
    public:
        int gregorianYear;
        GregorianDate firstNewMoon;
        constexpr LunarYear(int gregorianYear, const GregorianDate &firstNewMoon)
        {
            this->gregorianYear = gregorianYear;
            this->firstNewMoon = firstNewMoon;
            if(firstNewMoon.year != gregorianYear - 1) {
                throw std::runtime_error("first new moon should be for the year _prior_");
            }
            if(firstNewMoon.month != GregorianMonth::December) {
                throw std::runtime_error("First new moon should be the december in the prior year");
            }
        }


        constexpr std::vector<LunarPhase> getPhases() const
        {
            double daysElapsed = (31 - firstNewMoon.day);

            int daysThisYear = 365 + (yearIsLeapYear(this->gregorianYear) ? 1 : 0);
            std::vector<LunarPhase> phases;
            phases.reserve(daysThisYear);
            for(int i = 0; i < daysThisYear; i++) {
                daysElapsed += 1.0;
                double lunationsElapsed = daysElapsed / LUNATION_LENGTH_DAYS;
                double daysInCurrentLunation = std::fmod(daysElapsed, LUNATION_LENGTH_DAYS);
                double percentageThroughLunation = daysInCurrentLunation / LUNATION_LENGTH_DAYS;

                // Only 1 day should be a "new moon", a "full moon", (maybe even 1st and  last quarters?), so
                // we need to plop them out
                if(percentageThroughLunation <= .02) {
                    phases.push_back(LunarPhase::NewMoon);
                } else if(std::abs(percentageThroughLunation - .5) < .01) {
                    phases.push_back(LunarPhase::FullMoon);
                } else if(std::abs(percentageThroughLunation - .75) < .01) {
                    phases.push_back(LunarPhase::LastQuarter);
                } else if(std::abs(percentageThroughLunation - .25) < .01) {
                    phases.push_back(LunarPhase::FirstQuarter);
                } else {
                    int bucket = std::floor(percentageThroughLunation * UNIQUE_PHASE_COUNT);
                    // bump out of buckets covered already, the phases that are 1-day events, as opposed to multi-day periods of time
                    switch(static_cast<LunarPhase>(bucket)) {
                        case LunarPhase::NewMoon:
                        case LunarPhase::FirstQuarter:
                        case LunarPhase::FullMoon:
                        case LunarPhase::LastQuarter:
                            bucket += 1;
                            break;
                        default:
                            break;
                    }
                    phases.push_back(static_cast<LunarPhase>(bucket));
                }

            }

            return phases;
        }};
