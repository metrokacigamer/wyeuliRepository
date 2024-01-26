#include "side.h"
#include "corner.h"
    std::vector<std::string>& side::DrawLineInStringVec(std::vector<std::string> &arg)
    {
        arg[this->edges.first->coords.first][this->edges.first->coords.second] = '+'; // this->edges.first.;
        arg[this->edges.second->coords.first][this->edges.second->coords.second] = '+';
        if (this->isHoris)
        {
            bool SecondIsGreaterThanFirst = (this->edges.first->coords.second < this->edges.second->coords.second);
            size_t x1{SecondIsGreaterThanFirst ? this->edges.first->coords.second + 1 : this->edges.second->coords.second + 1};
            size_t x2{SecondIsGreaterThanFirst ? this->edges.second->coords.second : this->edges.first->coords.second};
            for (size_t i{this->edges.first->coords.first}; x1 < x2; ++x1)
            {
                arg[i][x1] = '-';
            }
        }
        else
        {
            bool SecondIsGreaterThanFirst = (this->edges.first->coords.first < this->edges.second->coords.first);
            size_t y1{SecondIsGreaterThanFirst ? this->edges.first->coords.first + 1 : this->edges.second->coords.first + 1};
            size_t y2{SecondIsGreaterThanFirst ? this->edges.second->coords.first : this->edges.first->coords.first};
            for (size_t j{this->edges.first->coords.second}; y1 < y2; ++y1)
            {
                arg[y1][j] = '|';
            }
        }
        return arg;
    }