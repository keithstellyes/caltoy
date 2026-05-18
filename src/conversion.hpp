#pragma once

#include "ethiopian.hpp"
#include "gregorian.hpp"
#include "julian.hpp"

ethiopian::Date toEthiopian(const gregorian::Date &d)
{
    return ethiopian::Date(d.getEpoch());
}

gregorian::Date toGregorian(const ethiopian::Date &d)
{
    return gregorian::Date(d.getEpoch());
}

julian::Date toJulian(const gregorian::Date &d)
{
    return julian::Date(d.getEpoch());
}

gregorian::Date toGregorian(const julian::Date &d)
{
    return gregorian::Date(d.getEpoch());
}
