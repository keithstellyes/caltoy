#include "LunarPhase.hpp"
#include <iostream>

void printYear(LunarYear ly)
{
    for(const LunarPhase &phase : ly.getPhases()) {
        std::cout << phase << '\n';
    }
}

int main(int argc, const char *argv[])
{
    printYear(LunarYear(2026, GregorianDate(2025, GregorianMonth::December, 19)));
}
