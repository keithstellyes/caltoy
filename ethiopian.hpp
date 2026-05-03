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

    int daysInMonth(Month m, bool isLeapYear)
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

    class Date
    {
        public:
            int year;
            Month month;
            uint8_t day;
    }
}
