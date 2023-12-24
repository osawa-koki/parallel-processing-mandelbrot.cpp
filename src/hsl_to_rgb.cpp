#include <array>
#include <cmath>
#include <iostream>

#include "interface.hpp"

RGB *hsl_to_rgb(HSL *hsl)
{
    int MAX_COLOR_VALUE = 255;

    int h = hsl->h;
    double s = hsl->s / 100.0;
    double l = hsl->l / 100.0;

    double max = l + (s * std::min(l, 1 - l));
    double min = l - (s * std::min(l, 1 - l));

    RGB *rgb = new RGB();
    int i = h / 60;

    switch (i)
    {
    case 0:
    case 6:
        rgb->r = max * MAX_COLOR_VALUE;
        rgb->g = (min + (max - min) * (h % 60) / 60) * MAX_COLOR_VALUE;
        rgb->b = min * MAX_COLOR_VALUE;
        break;
    case 1:
        rgb->r = (min + (max - min) * (60 - h % 60) / 60) * MAX_COLOR_VALUE;
        rgb->g = max * MAX_COLOR_VALUE;
        rgb->b = min * MAX_COLOR_VALUE;
        break;
    case 2:
        rgb->r = min * MAX_COLOR_VALUE;
        rgb->g = max * MAX_COLOR_VALUE;
        rgb->b = (min + (max - min) * (h % 60) / 60) * MAX_COLOR_VALUE;
        break;
    case 3:
        rgb->r = min * MAX_COLOR_VALUE;
        rgb->g = (min + (max - min) * (60 - h % 60) / 60) * MAX_COLOR_VALUE;
        rgb->b = max * MAX_COLOR_VALUE;
        break;
    case 4:
        rgb->r = (min + (max - min) * (h % 60) / 60) * MAX_COLOR_VALUE;
        rgb->g = min * MAX_COLOR_VALUE;
        rgb->b = max * MAX_COLOR_VALUE;
        break;
    case 5:
        rgb->r = max * MAX_COLOR_VALUE;
        rgb->g = min * MAX_COLOR_VALUE;
        rgb->b = (min + (max - min) * (60 - h % 60) / 60) * MAX_COLOR_VALUE;
        break;
    }

    return rgb;
}
