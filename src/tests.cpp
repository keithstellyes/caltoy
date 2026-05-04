#include <gtest/gtest.h>
#include "lunar.hpp"
#include "easter.hpp"
#include "conversion.hpp"

static void ExpectDateEq(const gregorian::Date& d, int y, gregorian::Month m, uint8_t day)
{
    EXPECT_EQ(d.year, y);
    EXPECT_EQ(d.month, m);
    EXPECT_EQ(d.day, day);
}

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

TEST(GregorianDateTest, AddDays_SameMonth)
{
    gregorian::Date d(2024, gregorian::Month::March, 10);
    auto result = d.addDays(5);

    ExpectDateEq(result, 2024, gregorian::Month::March, 15);
}

TEST(GregorianDateTest, AddDays_FirstNextMonth)
{
    gregorian::Date d(2026, gregorian::Month::January, 1);
    EXPECT_EQ(d.addDays(31), gregorian::Date(2026, gregorian::Month::February, 1));
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
    d = gregorian::Date(2026, gregorian::Month::May, 3);
    EXPECT_EQ(d.getDayOfYear(), 123);
}

TEST(GregorianDateTest, Epoch)
{
    EXPECT_EQ(gregorian::Date(1990, gregorian::Month::January, 1).getEpoch(), 0);
    EXPECT_EQ(gregorian::Date(2026, gregorian::Month::May, 3).getEpoch(), 13271);
    EXPECT_EQ(gregorian::Date((epoch_t)0), gregorian::Date(1990, gregorian::Month::January, 1));
    EXPECT_FALSE(gregorian::Date(1990, gregorian::Month::January, 1).isLeapYear());
    EXPECT_EQ(gregorian::Date((epoch_t)365), gregorian::Date(1991, gregorian::Month::January, 1));
    EXPECT_EQ(gregorian::Date((epoch_t)13271), gregorian::Date(2026, gregorian::Month::May, 3));

    for(epoch_t i = 0; i < 30000; i++) {
        gregorian::Date d(i);
        EXPECT_EQ(d, gregorian::Date(d.getEpoch()));
        if(i > 0) {
            EXPECT_TRUE(gregorian::Date(i - 1) < gregorian::Date(i));
        }
    }
}

TEST(GregorianDateTest, DateArithmetic)
{
    EXPECT_EQ(gregorian::Date(2026, gregorian::Month::June, 30) - gregorian::Date(2026, gregorian::Month::June, 5), 25);
    EXPECT_EQ(gregorian::Date(2026, gregorian::Month::June, 5) - gregorian::Date(2026, gregorian::Month::June, 30), -25);
}

TEST(GregorianDateTest, FromDayOfYear)
{
    EXPECT_EQ(gregorian::Date::fromDayOfYear(2026, 32), gregorian::Date(2026, gregorian::Month::February, 1));
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

TEST(EthiopianDateTest, Epoch)
{
    EXPECT_EQ(ethiopian::Date(1982, ethiopian::Month::Tahsas, 23).getEpoch(), 0);
    EXPECT_EQ(ethiopian::Date((epoch_t)0), ethiopian::Date(1982, ethiopian::Month::Tahsas, 23));
    EXPECT_EQ(ethiopian::Date((epoch_t)8), ethiopian::Date(1982, ethiopian::Month::Tir, 1));
    EXPECT_EQ(ethiopian::Date((epoch_t)253), ethiopian::Date(1983, ethiopian::Month::Meskerem, 1));
    for(epoch_t i = 0; i < 30000; i++) {
        ethiopian::Date d(i);
        EXPECT_EQ(d, ethiopian::Date(d.getEpoch()));
        if(i > 0) {
            EXPECT_TRUE(ethiopian::Date(i - 1) < ethiopian::Date(i));
        }
    }
}

TEST(ConversionTests, Ethiopian)
{
    ethiopian::Date d = toEthiopian(gregorian::Date(2026, gregorian::Month::September, 11));
    EXPECT_EQ(d, ethiopian::Date(2019, ethiopian::Month::Meskerem, 1));
    EXPECT_EQ(ethiopian::fromDayOfYear(2018, 235), ethiopian::Date(ethiopian::Date(2018, ethiopian::Month::Miyazya, 25)));
    d = toEthiopian(gregorian::Date(2026, gregorian::Month::September, 12));
    EXPECT_EQ(d, ethiopian::Date(2019, ethiopian::Month::Meskerem, 2));
    d = toEthiopian(gregorian::Date(2026, gregorian::Month::May, 4));
    EXPECT_EQ(d, ethiopian::Date(2018, ethiopian::Month::Miyazya, 26));
    EXPECT_EQ(ethiopian::fromDayOfYear(2018, 31), ethiopian::Date(2018, ethiopian::Month::Tikimt, 1));
    EXPECT_EQ(toGregorian(ethiopian::Date(1982, ethiopian::Month::Tahsas, 23)), gregorian::Date(1990, gregorian::Month::January, 1));
    for(epoch_t i = 0; i < 30000; i++) {
        ethiopian::Date ed(i);
        gregorian::Date gd(i);
        EXPECT_EQ(ed.getEpoch(), gd.getEpoch());
        EXPECT_EQ(ed.getEpoch(), i);
        EXPECT_EQ(ed, toEthiopian(gd));
        EXPECT_EQ(gd, toGregorian(ed));
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
