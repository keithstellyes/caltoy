#pragma once
/*
 * for calendars that were used after the reforms of Julius Caesar in 46 BC
 * The Julian and Gregorian calendars are the exact same except for two differences:
 * 1) the leap year determination is different
 * 2) The Gregorian calendar is some days ahead
 */
namespace julianreformed {
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

    enum class DayOfWeek {
        Sunday,
        Monday,
        Tuesday,
        Wednesday,
        Thursday,
        Friday,
        Saturday
    };

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
    class Date
    {
        protected:
            Date() {}
        public:
            int year;
            Month month;
            uint8_t day;
    };
}
constexpr std::string_view to_string(julianreformed::Month month)
{
    switch (month)
    {
        case julianreformed::Month::January:   return "January";
        case julianreformed::Month::February:  return "February";
        case julianreformed::Month::March:     return "March";
        case julianreformed::Month::April:     return "April";
        case julianreformed::Month::May:       return "May";
        case julianreformed::Month::June:      return "June";
        case julianreformed::Month::July:      return "July";
        case julianreformed::Month::August:    return "August";
        case julianreformed::Month::September: return "September";
        case julianreformed::Month::October:   return "October";
        case julianreformed::Month::November:  return "November";
        case julianreformed::Month::December:  return "December";
    }

    return "Unknown";
}
inline std::ostream& operator<<(std::ostream& os, julianreformed::Month month)
{
    return os << to_string(month);
}

