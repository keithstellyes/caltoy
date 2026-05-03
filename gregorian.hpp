#pragma once

#include <cstdint>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace gregorian {
    enum class Month {
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

    constexpr std::vector<Month> getAllMonths()
    {
        std::vector<Month> months;
        for(int i = 0; i < 12; i++) {
            months.push_back(static_cast<Month>(i));
        }
        return months;
    }
    inline Month& operator++(Month& m)
    {
        m = static_cast<Month>((static_cast<int>(m) + 1) % 12);
        return m;
    }

    inline Month operator++(Month& m, int)
    {
        Month old = m;
        ++m;
        return old;
    }

    constexpr std::string_view to_string(Month month)
    {
        switch (month)
        {
            case Month::January:   return "January";
            case Month::February:  return "February";
            case Month::March:     return "March";
            case Month::April:     return "April";
            case Month::May:       return "May";
            case Month::June:      return "June";
            case Month::July:      return "July";
            case Month::August:    return "August";
            case Month::September: return "September";
            case Month::October:   return "October";
            case Month::November:  return "November";
            case Month::December:  return "December";
        }

        return "Unknown";
    }
    inline std::ostream& operator<<(std::ostream& os, Month month)
    {
        return os << to_string(month);
    }

    constexpr uint8_t daysInMonth(Month month, bool isLeapYear)
    {
        switch(month)
        {
            case Month::January:
                return 31;
            case Month::February:
                return 28 + (isLeapYear ? 1 : 0);
            case Month::March:
                return 31;
            case Month::April:
                return 30;
            case Month::May:
                return 31;
            case Month::June:
                return 30;
            case Month::July:
                return 31;
            case Month::August:
                return 31;
            case Month::September:
                return 30;
            case Month::October:
                return 31;
            case Month::November:
                return 30;
            case Month::December:
                return 31;
            default:
                return -1;
        }
    }

    constexpr bool yearIsLeapYear(int year)
    {
        return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
    }

    enum class DayOfWeek {
        Sunday,
        Monday,
        Tuesday,
        Wednesday,
        Thursday,
        Friday,
        Saturday
    };

    class Date
    {
        public:
            int year;
            Month month;
            uint8_t day;
            constexpr Date() : year(2026), month(Month::January), day(1) {}
            constexpr Date(int year, Month month, uint8_t day) : year(year), month(month), day(day)
        {
            if(day == 0) {
                throw std::runtime_error("All days in a  calendar must be at least 1!");
            }
            if(day > daysInMonth(month, yearIsLeapYear(year))) {
                throw std::runtime_error("Not that many days in this month");
            }
        }
            constexpr bool isLeapYear() const
            {
                return yearIsLeapYear(year);
            }

            // naive algorithm
            constexpr Date addDays(int days) const
            {
                if(days < 0) {
                    throw std::runtime_error("Not yet implemented");
                }
                int newDayInMonth = days + this->day;
                if(newDayInMonth <= daysInMonth(month, isLeapYear())) {
                    return Date(year, month, newDayInMonth);
                }
                Month nextMonth = static_cast<Month>((static_cast<int>(month) + 1) % 12);
                int nextYear = year + (nextMonth == Month::January ? 1 : 0);
                int daysToReachEndOfMonth = daysInMonth(month, isLeapYear()) - this->day + 1;
                return Date(nextYear, nextMonth, 1).addDays(days - daysToReachEndOfMonth);
            }

            // Sakamoto algorithm? https://stackoverflow.com/a/905468
            constexpr DayOfWeek getDayOfWeek() const
            {
                int m = static_cast<int>(month) + 1;
                int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
                int y = year;
                y -= m < 3;
                return static_cast<DayOfWeek>((y + y/4 - y/100 + y/400 + t[m-1] + day) % 7);
            }
            constexpr int getDayOfYear() const
            {
                int daysPriorToThisMonth = 0;
                Month currMonth = Month::January;
                while(currMonth != month) {
                    daysPriorToThisMonth += daysInMonth(currMonth, isLeapYear());
                    currMonth++;
                }
                return daysPriorToThisMonth + day;
            }
    };

    inline bool operator==(const Date &lhs, const Date &rhs)
    {
        return lhs.year == rhs.year && lhs.month == rhs.month && lhs.day == rhs.day;
    }
}
