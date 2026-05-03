#include <gtest/gtest.h>
#include "lunar.hpp"
#include "easter.hpp"

// --- Helper for comparisons ---
static void ExpectDateEq(const gregorian::Date& d, int y, gregorian::Month m, uint8_t day)
{
    EXPECT_EQ(d.year, y);
    EXPECT_EQ(d.month, m);
    EXPECT_EQ(d.day, day);
}

// --- Constructor tests ---

TEST(GregorianDateTest, DefaultConstructor)
{
    gregorian::Date d;
    ExpectDateEq(d, 2026, gregorian::Month::January, 1);
}

TEST(GregorianDateTest, ParameterizedConstructor)
{
    gregorian::Date d(2024, gregorian::Month::March, 15);
    ExpectDateEq(d, 2024, gregorian::Month::March, 15);
}

// --- Leap year tests ---

TEST(GregorianDateTest, LeapYear_True)
{
    gregorian::Date d(2024, gregorian::Month::January, 1);
    EXPECT_TRUE(d.isLeapYear());
}

TEST(GregorianDateTest, LeapYear_False)
{
    gregorian::Date d(2023, gregorian::Month::January, 1);
    EXPECT_FALSE(d.isLeapYear());
}

// --- addDays basic tests ---

TEST(GregorianDateTest, AddDays_SameMonth)
{
    gregorian::Date d(2024, gregorian::Month::March, 10);
    auto result = d.addDays(5);

    ExpectDateEq(result, 2024, gregorian::Month::March, 15);
}

TEST(GregorianDateTest, AddDays_NextDay)
{
    gregorian::Date d(2024, gregorian::Month::March, 10);
    auto result = d.addDays(1);

    ExpectDateEq(result, 2024, gregorian::Month::March, 11);
}

TEST(GregorianDateTest, AddDays_CrossMonth)
{
    gregorian::Date d(2024, gregorian::Month::March, 28);
    auto result = d.addDays(5);

    ExpectDateEq(result, 2024, gregorian::Month::April, 2);
}

// --- February / leap year edge cases ---

TEST(GregorianDateTest, AddDays_February_NonLeap)
{
    gregorian::Date d(2023, gregorian::Month::February, 27);
    auto result = d.addDays(2);

    ExpectDateEq(result, 2023, gregorian::Month::March, 1);
}

TEST(GregorianDateTest, AddDays_February_Leap)
{
    gregorian::Date d(2024, gregorian::Month::February, 27);
    auto result = d.addDays(2);

    ExpectDateEq(result, 2024, gregorian::Month::February, 29);
}

TEST(GregorianDateTest, AddDays_February_Leap_Cross)
{
    gregorian::Date d(2024, gregorian::Month::February, 28);
    auto result = d.addDays(2);

    ExpectDateEq(result, 2024, gregorian::Month::March, 1);
}

// --- Year boundary ---

TEST(GregorianDateTest, AddDays_EndOfYear)
{
    gregorian::Date d(2023, gregorian::Month::December, 31);
    auto result = d.addDays(1);

    ExpectDateEq(result, 2024, gregorian::Month::January, 1);
}


TEST(GregorianDateTest, AddDays_LargeSpan)
{
    gregorian::Date d(2023, gregorian::Month::January, 1);
    auto result = d.addDays(365);

    // 2023 is not a leap year
    ExpectDateEq(result, 2024, gregorian::Month::January, 1);
}

TEST(GregorianDateTest, AddDays_LargeSpan_LeapYear)
{
    gregorian::Date d(2024, gregorian::Month::January, 1);
    auto result = d.addDays(366);

    ExpectDateEq(result, 2025, gregorian::Month::January, 1);
}

TEST(GregorianDateTest, DayOfWeek)
{
    gregorian::Date d(1995, gregorian::Month::June, 30);
    EXPECT_EQ(d.getDayOfWeek(), gregorian::DayOfWeek::Friday);
}

TEST(GregorianDateTest, DayOfYear)
{
    gregorian::Date d(2026, gregorian::Month::December, 31);
    EXPECT_EQ(d.getDayOfYear(), 365);
}

TEST(LunarPhaseTest, LunarPhaseTable)
{
    EXPECT_EQ(lunar::lookupLastNewMoonInPriorYear(2026).value(), gregorian::Date(2025, gregorian::Month::December, 19));
}

TEST(LunarPhaseTest, LunarPhaseCalculation)
{
    EXPECT_EQ(lunar::Year(2026).getPhases()[2], lunar::Phase::FullMoon);
    EXPECT_EQ(lunar::Year(2026).getPhases()[364], lunar::Phase::WaningCrescent);
    EXPECT_EQ(lunar::Year(2026).getPhase(gregorian::Date(2026, gregorian::Month::December, 31)), lunar::Phase::WaningCrescent);
}

TEST(EasterTest, EasterCalc)
{
    EXPECT_EQ(gregorian::getEaster(2025), gregorian::Date(2025, gregorian::Month::April, 20));
    EXPECT_EQ(gregorian::getEaster(2026), gregorian::Date(2026, gregorian::Month::April, 5));
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
