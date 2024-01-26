#ifndef SIDE_H
#define SIDE_H
#include <string>
#include <vector>
struct corner;

struct side
{
    std::pair<corner *, corner *> edges;
    bool isBoundary{};
    bool isHoris{};
    std::vector<std::string> &DrawLineInStringVec(std::vector<std::string> &arg);

    side() = default;
};

#endif