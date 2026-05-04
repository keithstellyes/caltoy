#pragma once

#include "ethiopian.hpp"
#include "gregorian.hpp"

ethiopian::Date toEthiopian(const gregorian::Date &d)
{
    return ethiopian::Date(d.getEpoch());
}

gregorian::Date toGregorian(const ethiopian::Date &d)
{
    return gregorian::Date(d.getEpoch());
}
