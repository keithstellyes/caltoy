#pragma once
#include "gregorian.hpp"
#include <cstdint>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>
#define LUNATION_LENGTH_DAYS 29.53

namespace lunar {
    enum class Phase {
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

    constexpr std::string_view to_string(Phase phase)
    {
        switch (phase)
        {
            case Phase::NewMoon:         return "New Moon";
            case Phase::WaxingCrescent:  return "Waxing Crescent";
            case Phase::FirstQuarter:    return "First Quarter";
            case Phase::WaxingGibbous:   return "Waxing Gibbous";
            case Phase::FullMoon:        return "Full Moon";
            case Phase::WaningGibbous:   return "Waning Gibbous";
            case Phase::LastQuarter:     return "Last Quarter";
            case Phase::WaningCrescent:  return "Waning Crescent";
        }

        return "Unknown"; // fallback for invalid values
    }

    constexpr char32_t unicodeCodepoint(Phase phase)
    {
        return 127761 + static_cast<int>(phase);
    }


    inline std::ostream& operator<<(std::ostream& os, Phase phase)
    {
        return os << to_string(phase);
    }

    std::optional<gregorian::Date> lookupLastNewMoonInPriorYear(int year)
    {
        int day = 0;
        switch(year) {
            case 2026:
                day = 19;
                break;
            case 2025:
                day = 30;
                break;
            default:
                day = 0;
                break;
        }

        if(day) {
            return gregorian::Date(year - 1, gregorian::Month::December, day);
        }
        return std::nullopt;
    }

    class Year {
        public:
            int gregorianYear;
            gregorian::Date firstNewMoon;
            constexpr Year(int gregorianYear, const gregorian::Date &firstNewMoon)
            {
                this->gregorianYear = gregorianYear;
                this->firstNewMoon = firstNewMoon;
                if(firstNewMoon.year != gregorianYear - 1) {
                    throw std::runtime_error("first new moon should be for the year _prior_");
                }
                if(firstNewMoon.month != gregorian::Month::December) {
                    throw std::runtime_error("First new moon should be the december in the prior year");
                }
            }
            constexpr Year(int gregorianYear)
            {
                this->gregorianYear = gregorianYear;
                this->firstNewMoon = lookupLastNewMoonInPriorYear(gregorianYear).value();
            }

            constexpr Phase getPhase(const gregorian::Date &d)
            {
                if(d.year != gregorianYear) {
                    throw std::runtime_error("Gregorian year out of scope!");
                }
                return getPhases()[d.getDayOfYear() - 1];
            }

            constexpr std::vector<Phase> getPhases() const
            {
                double daysElapsed = (31 - firstNewMoon.day);

                int daysThisYear = 365 + (gregorian::yearIsLeapYear(this->gregorianYear) ? 1 : 0);
                std::vector<Phase> phases;
                phases.reserve(daysThisYear);
                for(int i = 0; i < daysThisYear; i++) {
                    daysElapsed += 1.0;
                    // maybe unused, calculated to make code/algorithm easier to understand
                    [[maybe_unused]] double lunationsElapsed = daysElapsed / LUNATION_LENGTH_DAYS;
                    double daysInCurrentLunation = std::fmod(daysElapsed, LUNATION_LENGTH_DAYS);
                    double percentageThroughLunation = daysInCurrentLunation / LUNATION_LENGTH_DAYS;

                    // Only 1 day should be a "new moon", a "full moon", (maybe even 1st and  last quarters?), so
                    // we need to plop them out
                    if(percentageThroughLunation <= .02) {
                        phases.push_back(Phase::NewMoon);
                    } else if(std::abs(percentageThroughLunation - .5) < .01) {
                        phases.push_back(Phase::FullMoon);
                    } else if(std::abs(percentageThroughLunation - .75) < .01) {
                        phases.push_back(Phase::LastQuarter);
                    } else if(std::abs(percentageThroughLunation - .25) < .01) {
                        phases.push_back(Phase::FirstQuarter);
                    } else {
                        int bucket = std::floor(percentageThroughLunation * UNIQUE_PHASE_COUNT);
                        // bump out of buckets covered already, the phases that are 1-day events, as opposed to multi-day periods of time
                        switch(static_cast<Phase>(bucket)) {
                            case Phase::NewMoon:
                            case Phase::FirstQuarter:
                            case Phase::FullMoon:
                            case Phase::LastQuarter:
                                bucket += 1;
                                break;
                            default:
                                break;
                        }
                        phases.push_back(static_cast<Phase>(bucket));
                        if(phases.size() > 1) {
                            if(phases[phases.size() - 1] == Phase::WaningGibbous && phases[phases.size() - 2] == Phase::WaxingGibbous) {
                                // sometimes waxing skips to waning, we'll need to do for the other "just 1 day" cases like last quarter & first quarter too I bet
                                phases[phases.size() - 1] = Phase::FullMoon;
                            } else if(phases[phases.size() - 1] == Phase::WaxingCrescent && phases[phases.size() - 2] == Phase::WaningCrescent) {
                                phases[phases.size() - 1] = Phase::NewMoon;
                            } else if(phases[phases.size() - 1] == Phase::WaxingGibbous && phases[phases.size() - 2] == Phase::WaxingCrescent) {
                                phases[phases.size() - 1] = Phase::FirstQuarter;
                            }
                        }
                    }

                }

                return phases;
            }
    };
}
