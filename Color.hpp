#pragma once

#include <compare>

struct Color
{
    int r {0};
    int g {0};
    int b {0};
    int alpha {255};

    constexpr Color() = default;
    constexpr explicit Color(int nr, int ng, int nb, int nalpha)
    {
        if (nr >= 0 && nr <= 255) r = nr;
        if (ng >= 0 && ng <= 255) g = ng;
        if (nb >= 0 && nb <= 255) b = nb;
        if (nalpha >= 0 && nalpha <= 255) alpha = nalpha;
    }

    auto operator<=>(const Color& color) const = default;
};

