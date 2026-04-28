#include <gtest/gtest.h>
#include "LunarPhase.hpp"

// --- Helper for comparisons ---
static void ExpectDateEq(const GregorianDate& d, int y, GregorianMonth m, uint8_t day)
{
    EXPECT_EQ(d.year, y);
    EXPECT_EQ(d.month, m);
    EXPECT_EQ(d.day, day);
}

// --- Constructor tests ---

TEST(GregorianDateTest, DefaultConstructor)
{
    GregorianDate d;
    ExpectDateEq(d, 2026, GregorianMonth::January, 1);
}

TEST(GregorianDateTest, ParameterizedConstructor)
{
    GregorianDate d(2024, GregorianMonth::March, 15);
    ExpectDateEq(d, 2024, GregorianMonth::March, 15);
}

// --- Leap year tests ---

TEST(GregorianDateTest, LeapYear_True)
{
    GregorianDate d(2024, GregorianMonth::January, 1);
    EXPECT_TRUE(d.isLeapYear());
}

TEST(GregorianDateTest, LeapYear_False)
{
    GregorianDate d(2023, GregorianMonth::January, 1);
    EXPECT_FALSE(d.isLeapYear());
}

// --- addDays basic tests ---

TEST(GregorianDateTest, AddDays_SameMonth)
{
    GregorianDate d(2024, GregorianMonth::March, 10);
    auto result = d.addDays(5);

    ExpectDateEq(result, 2024, GregorianMonth::March, 15);
}

TEST(GregorianDateTest, AddDays_NextDay)
{
    GregorianDate d(2024, GregorianMonth::March, 10);
    auto result = d.addDays(1);

    ExpectDateEq(result, 2024, GregorianMonth::March, 11);
}

TEST(GregorianDateTest, AddDays_CrossMonth)
{
    GregorianDate d(2024, GregorianMonth::March, 28);
    auto result = d.addDays(5);

    ExpectDateEq(result, 2024, GregorianMonth::April, 2);
}

// --- February / leap year edge cases ---

TEST(GregorianDateTest, AddDays_February_NonLeap)
{
    GregorianDate d(2023, GregorianMonth::February, 27);
    auto result = d.addDays(2);

    ExpectDateEq(result, 2023, GregorianMonth::March, 1);
}

TEST(GregorianDateTest, AddDays_February_Leap)
{
    GregorianDate d(2024, GregorianMonth::February, 27);
    auto result = d.addDays(2);

    ExpectDateEq(result, 2024, GregorianMonth::February, 29);
}

TEST(GregorianDateTest, AddDays_February_Leap_Cross)
{
    GregorianDate d(2024, GregorianMonth::February, 28);
    auto result = d.addDays(2);

    ExpectDateEq(result, 2024, GregorianMonth::March, 1);
}

// --- Year boundary ---

TEST(GregorianDateTest, AddDays_EndOfYear)
{
    GregorianDate d(2023, GregorianMonth::December, 31);
    auto result = d.addDays(1);

    ExpectDateEq(result, 2024, GregorianMonth::January, 1);
}

// --- Large additions ---

TEST(GregorianDateTest, AddDays_LargeSpan)
{
    GregorianDate d(2023, GregorianMonth::January, 1);
    auto result = d.addDays(365);

    // 2023 is not a leap year
    ExpectDateEq(result, 2024, GregorianMonth::January, 1);
}

TEST(GregorianDateTest, AddDays_LargeSpan_LeapYear)
{
    GregorianDate d(2024, GregorianMonth::January, 1);
    auto result = d.addDays(366);

    ExpectDateEq(result, 2025, GregorianMonth::January, 1);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
