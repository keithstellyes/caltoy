#pragma once
#include "julianreformed.hpp"

namespace julian {
    using Month = julianreformed::Month;
    constexpr uint8_t daysInMonth(Month month, bool isLeapYear)
    {
        return julianreformed::daysInMonth(month, isLeapYear);
    }

    constexpr std::vector<Month> getAllMonths()
    {
        std::vector<Month> months;
        for(int i = 0; i < 12; i++) {
            months.push_back(static_cast<Month>(i));
        }
        return months;
    }
    const int EPOCH_YEAR = 1989;
    const Month EPOCH_MONTH = Month::December;
    const uint8_t EPOCH_DAY = 19;
    constexpr bool yearIsLeapYear(int year)
    {
        return year % 4 == 0;
    }

    constexpr int daysInYear(int year)
    {
        return 365 + (yearIsLeapYear(year) ? 1 : 0);
    }
    using DayOfWeek = julianreformed::DayOfWeek;
    class Date : public julianreformed::Date
    {
        public:
            constexpr Date(int year, Month month, uint8_t day)
            {
                this->year = year;
                this->month = month;
                this->day = day;
                if(day == 0) {
                    throw std::runtime_error("All days in a calendar must be at least 1!");
                }
                if(day > julian::daysInMonth(month, yearIsLeapYear(year))) {
                    throw std::runtime_error("Not that many days in this month");
                }
            }
            constexpr static Date fromDayOfYear(int julianYear, int dayOfYear)
            {
                if(dayOfYear < 1 || dayOfYear > daysInYear(julianYear) || julianYear < 1500) {
                    throw std::runtime_error("Bad input for fromDayOfYear... got dayOfYear=" + std::to_string(dayOfYear) + ", julianYear=" + std::to_string(julianYear));
                }
                bool isLy = yearIsLeapYear(julianYear);
                for(const Month &month : getAllMonths()) {
                    if(julian::daysInMonth(month, isLy) >= dayOfYear) {
                        return julian::Date(julianYear, month, dayOfYear);
                    }
                    dayOfYear -= julian::daysInMonth(month, isLy);
                }
                assert(false);
            }

            constexpr Date(epoch_t epoch)
            {
                if(epoch < 0) {
                    throw std::runtime_error("Not yet implemented");
                }
                int year = EPOCH_YEAR;
                while(daysInYear(year) - 1 < epoch) {
                    epoch -= daysInYear(year);
                    year++;
                }
                Date target = fromDayOfYear(year, epoch + 1);
                *this = target;
            }
            constexpr bool isLeapYear() const
            {
                return yearIsLeapYear(year);
            }
            constexpr epoch_t getEpoch() const
            {
                if(year < EPOCH_YEAR) {
                    throw std::runtime_error("Not yet implemented!");
                }
                int days = 0;
                // naive
                for(int y = EPOCH_YEAR; y < year; y++) {
                    days += daysInYear(y);
                }
                return days + getDayOfYear() - 1;
            }
            bool operator==(const Date& other) const
            {
                return year == other.year &&
                    month == other.month &&
                    day == other.day;
            }

            bool operator<(const Date& other) const
            {
                if (year != other.year)
                    return year < other.year;

                if (month != other.month)
                    return month < other.month;

                return day < other.day;
            }

            bool operator!=(const Date& other) const
            {
                return !(*this == other);
            }

            bool operator>(const Date& other) const
            {
                return other < *this;
            }

            bool operator<=(const Date& other) const
            {
                return !(*this > other);
            }

            bool operator>=(const Date& other) const
            {
                return !(*this < other);
            }

            constexpr Date addDays(int days) const
            {
                return Date(getEpoch() + days);
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
                    daysPriorToThisMonth += julian::daysInMonth(currMonth, isLeapYear());
                    currMonth++;
                }
                return daysPriorToThisMonth + day;
            }

    };
}
