#ifndef CORNER_H
#define CORNER_H
#include "side.h"
struct side;

struct corner
{
    std::vector<side *> sides;
    std::pair<size_t, size_t> coords;
    corner(size_t y, size_t x) : coords(y, x) {}
    corner() = default;
};

#endif