#include "LunarPhase.hpp"
#include <iostream>

void printMonth(GregorianDate firstDay, const std::span<LunarPhase> &phases)
{
    std::cout << firstDay.month << '\n';
    std::cout << "S M T W T F S\n";
    for(int i = 0; i < static_cast<int>(firstDay.getDayOfWeek()); i++) {
        std::cout << "  ";
    }
    GregorianDate today = firstDay;
    int phaseIndex = 0;
    while(today.month == firstDay.month) {
        std::cout << to_utf8(unicodeCodepoint(phases[phaseIndex++]));
        if(today.getDayOfWeek() == GregorianDayOfWeek::Saturday) {
            if(today.day < daysInGregorianMonth(today.month, today.isLeapYear())) {
                std::cout << '\n';
            }
        }
        today = today.addDays(1);
    }
}

void printYear(LunarYear ly)
{
    GregorianDate today(ly.gregorianYear - 1, GregorianMonth::December, 31);
    auto phases = ly.getPhases();
    int phaseIndex = 0;
    for(int i = 0; i < 12; i++) {
        GregorianDate firstDay = GregorianDate(ly.gregorianYear, static_cast<GregorianMonth>(i), 1);
        std::span<LunarPhase> phasesThisMonth(phases.begin() + phaseIndex, daysInGregorianMonth(firstDay.month, firstDay.isLeapYear()));
        printMonth(firstDay, phasesThisMonth);
        phaseIndex += phasesThisMonth.size();
        std::cout << '\n';
    }
}

int main(int argc, const char *argv[])
{
    printYear(LunarYear(2026, GregorianDate(2025, GregorianMonth::December, 19)));
    std::cout << std::endl;
    return 0;
}
