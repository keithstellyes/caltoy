#pragma once

#include <string>

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
    Evangelist evangelistForYear(int year)
    {
        Evangelist result = static_cast<Evangelist>(year % 4);
        assert((result == Evangelist::Luke) == yearIsLeapYear(year));
        return result;
    }

    class Date
    {
        public:
            int year;
            Month month;
            uint8_t day;
            constexpr Date(int year, Month month, uint8_t day)
            {
                this->year = year;
                this->month = month;
                this->day = day;
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
}

