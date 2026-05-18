#pragma once

#include <iomanip>
#include <string>
#include "epoch.hpp"

namespace ethiopian {
    enum class Month
    {
        Meskerem,
        Tikimt,
        Hidar,
        Tahsas,
        Tir,
        Yakatit,
        Maggabit,
        Miyazya,
        Ginbot,
        Sene,
        Hamle,
        Nehasa,
        Pagume
    };

    enum class Evangelist
    {
        John,
        Matthew,
        Mark,
        Luke
    };

    const int EPOCH_YEAR = 1982;
    const Month EPOCH_MONTH = Month::Tahsas;
    const uint8_t EPOCH_DAY = 23;
    std::string to_string(Month m) {
        switch (m) {
            case Month::Meskerem: return "Meskerem";
            case Month::Tikimt:   return "Tikimt";
            case Month::Hidar:    return "Hidar";
            case Month::Tahsas:   return "Tahsas";
            case Month::Tir:      return "Tir";
            case Month::Yakatit:  return "Yakatit";
            case Month::Maggabit: return "Maggabit";
            case Month::Miyazya:  return "Miyazya";
            case Month::Ginbot:   return "Ginbot";
            case Month::Sene:     return "Sene";
            case Month::Hamle:    return "Hamle";
            case Month::Nehasa:   return "Nehasa";
            case Month::Pagume:   return "Pagume";
            default: return "Unknown";
        }
    }
    inline std::ostream& operator<<(std::ostream& os, const Month& m)
    {
        return os << to_string(m);
    }

    constexpr int daysInMonth(Month m, bool isLeapYear)
    {
        if (m == Month::Pagume) {
            return isLeapYear ? 6 : 5;
        }
        return 30;
    }
    bool yearIsLeapYear(int year)
    {
        return (year % 4 == 3);
    }
    const int DAYS_BETWEEN_EPOCH_AND_NEXT_YEAR = (30 - EPOCH_DAY) + (8 * 30) + daysInMonth(Month::Pagume, yearIsLeapYear(EPOCH_YEAR));

    Evangelist evangelistForYear(int year)
    {
        Evangelist result = static_cast<Evangelist>(year % 4);
        assert((result == Evangelist::Luke) == yearIsLeapYear(year));
        return result;
    }
    constexpr int daysInYear(int year)
    {
        return 365 + (yearIsLeapYear(year) ? 1 : 0);
    }
    class Date
    {
        private:
            constexpr Date nextDay() const
            {
                if(day + 1 <= daysInMonth(month, isLeapYear())) {
                    return Date(year, month, day + 1);
                }
                assert(day + 1 == (daysInMonth(month, isLeapYear()) + 1));
                Date result(*this);
                result.month = static_cast<Month>((static_cast<int>(month) + 1) % 13);
                result.day = 1;
                if(result.month == Month::Meskerem) {
                    result.year++;
                }
                return result;
            }
        public:
            int year;
            Month month;
            uint8_t day;
            constexpr Date(int year, Month month, uint8_t day)
            {
                this->year = year;
                this->month = month;
                this->day = day;
                if(day < 1 || day > 30) {
                    throw std::runtime_error("Impossible day!");
                }
            }

            constexpr static Date fromDayOfYear(int ethiopianYear, int dayOfYear)
            {
                if(dayOfYear < 1 || dayOfYear > 366) {
                    throw std::runtime_error("Invalid day-of-year for Ethiopian calendar: " + std::to_string(dayOfYear));
                }
                if(ethiopianYear <= 1900-8 || ethiopianYear >= 2100 - 8) {
                    throw std::runtime_error("Not year implemented");
                }
                if(dayOfYear <= 360)
                {
                    uint8_t day = ((dayOfYear - 1) % 30) + 1 /* adjust for -1 for mod logic to play nice*/;
                    Month month = static_cast<Month>((dayOfYear - 1) / 30);
                    return Date(ethiopianYear, month, day);
                }
                return Date(ethiopianYear, Month::Pagume, dayOfYear - 360);
            }
            constexpr epoch_t getEpoch() const
            {
                if(year < EPOCH_YEAR) {
                    throw std::runtime_error("Not yet implemented!");
                } else if(year == EPOCH_YEAR) {
                    if(*this == getEpochDate()) {
                        return 0;
                    } else if(month < EPOCH_MONTH) {
                        throw std::runtime_error("Not yet implemented!");
                    } else if(month == EPOCH_MONTH) {
                        return day - EPOCH_DAY;
                    } else {
                        return getDayOfYear() - getEpochDate().getDayOfYear();
                    }
                }
                int y = EPOCH_YEAR + 1;
                epoch_t epoch = DAYS_BETWEEN_EPOCH_AND_NEXT_YEAR;
                while(y < this->year) {
                    epoch += daysInYear(y);
                    y++;
                }
                return epoch + getDayOfYear();
            }
            constexpr Date addDays(int days) const
            {
                return Date(getEpoch() + days);
            }
            constexpr Date(epoch_t epoch)
            {
                if(epoch < 0) {
                    throw std::runtime_error("Not yet implemented");
                }
                Date result(EPOCH_YEAR, EPOCH_MONTH, EPOCH_DAY);
                while(epoch--) {
                    // naive
                    result = result.nextDay();
                }
                *this = result;
            }
            constexpr int getDayOfYear() const
            {
                if(month == Month::Pagume) {
                    return day + 12 * 30;;
                }
                return 30 * static_cast<int>(month) + day;
            }
            constexpr bool isLeapYear() const
            {
                return yearIsLeapYear(this->year);
            }
            constexpr static Date getEpochDate()
            {
                return Date(EPOCH_YEAR, EPOCH_MONTH, EPOCH_DAY);
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

    };
    std::string to_string(const Date& d)
    {
        std::ostringstream oss;
        oss << std::setfill('0')
            << std::setw(4) << d.year << '-'
            << to_string(d.month) << '-'
            << std::setw(2) << static_cast<int>(d.day);
        return oss.str();
    }

    inline std::ostream& operator<<(std::ostream& os, const Date& d)
    {
        return os << to_string(d);
    }

    constexpr Date fromDayOfYear(int ethiopianYear, int dayOfYear)
    {
        return Date::fromDayOfYear(ethiopianYear, dayOfYear);
    }
}

