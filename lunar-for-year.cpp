#include "lunar.hpp"
#include "util.hpp"
#include <iostream>

void printMonth(gregorian::Date firstDay, const std::span<lunar::Phase> &phases)
{
    std::cout << firstDay.month << '\n';
    std::cout << "S M T W T F S\n";
    for(int i = 0; i < static_cast<int>(firstDay.getDayOfWeek()); i++) {
        std::cout << "  ";
    }
    gregorian::Date today = firstDay;
    int phaseIndex = 0;
    while(today.month == firstDay.month) {
        std::cout << to_utf8(unicodeCodepoint(phases[phaseIndex++]));
        if(today.getDayOfWeek() == gregorian::DayOfWeek::Saturday) {
            if(today.day < gregorian::daysInMonth(today.month, today.isLeapYear())) {
                std::cout << '\n';
            }
        }
        today = today.addDays(1);
    }
}

void printYear(lunar::Year ly)
{
    gregorian::Date today(ly.gregorianYear - 1, gregorian::Month::December, 31);
    auto phases = ly.getPhases();
    int phaseIndex = 0;
    for(int i = 0; i < 12; i++) {
        gregorian::Date firstDay = gregorian::Date(ly.gregorianYear, static_cast<gregorian::Month>(i), 1);
        std::span<lunar::Phase> phasesThisMonth(phases.begin() + phaseIndex, gregorian::daysInMonth(firstDay.month, firstDay.isLeapYear()));
        printMonth(firstDay, phasesThisMonth);
        phaseIndex += phasesThisMonth.size();
        std::cout << '\n';
    }
}

int main(int argc, const char *argv[])
{
    int year = 2026;
    if(argc > 1) {
        year = std::stoi(argv[1]);
    }
    printYear(lunar::Year(year));
    std::cout << std::endl;
    return 0;
}
