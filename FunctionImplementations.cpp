#include "functions.h"

template <class T>
int erase(std::vector<T> &c, T value)
{
    auto it = std::remove(c.begin(), c.end(), value);
    auto r = c.end() - it;
    c.erase(it, c.end());
    return r;
}

std::vector<std::string> FillStringVector(std::vector<std::string> &strArr2)
{
    std::vector<std::string> strArr{strArr2};
    int max{};
    for (auto i : strArr)
    {
        max = (max > i.length()) ? max : i.length();
    }

    for (size_t i{}, j{}; i < strArr.size(); ++i)
    {
        if (strArr[i].length() < max)
        {
            std::string temp(max - strArr[i].length(), ' ');
            strArr[i].append(temp);
        }
    }
    return strArr;
}

bool isBoundary(side *line, std::vector<std::pair<int, int>> BoundaryCoordinates)
{
    bool IsBoundary{};
    size_t x1 = line->edges.first->coords.second;
    size_t x2 = line->edges.second->coords.second;
    size_t y1 = line->edges.first->coords.first;
    size_t y2 = line->edges.second->coords.first;
    bool FirstXIsSmaller = x1 < x2;
    bool FirstYIsSmaller = y1 < y2;
    std::pair<int, int> Coords;
    if (line->IsHoris)
    {
        if (FirstXIsSmaller)
            Coords = {y1, x1 + 1};
        else
            Coords = {y2, x2 + 1};
    }
    else
    {
        if (FirstYIsSmaller)
            Coords = {y1 + 1, x1};
        else
            Coords = {y2 + 1, x2};
    }
    for (auto i : BoundaryCoordinates)
    {
        IsBoundary = i.first == Coords.first && i.second == Coords.second ? true : IsBoundary;
        if (IsBoundary)
            return true;
    }
    return false;
}

std::pair<std::vector<corner *>, std::vector<side *>> Filter(std::vector<std::string> &arg2)
{
    std::vector<std::string> arg{arg2};
    auto X = arg[0].length();
    auto Y = arg.size();
    std::pair<std::vector<corner *>, std::vector<side *>> pair; // = new ;

    for (size_t i{}; i < arg.size(); ++i)
    {
        for (size_t j{}; j < arg[i].length(); ++j)
        {
            if (arg[i][j] == '+')
            {
                auto var = new corner(i, j);
                pair.first.push_back(var);
            }
            else if (arg[i][j] == '-')
            {
                auto var = new side();
                pair.second.push_back(var);
                pair.second.back()->IsHoris = true;
                // pair->second.back()->IsBoundary = isBoundary(arg, j, i);
                pair.second.back()->edges.first = pair.first.back();
                pair.first.back()->sides.push_back(pair.second.back());
                while (arg[i][j] != '+')
                {
                    ++j;
                }

                auto var2 = new corner(i, j);
                pair.first.push_back(var2);

                pair.second.back()->edges.second = pair.first.back();
                pair.first.back()->sides.push_back(pair.second.back());
            }
        }
    }
    for (size_t j{}, i{}; j < X; ++j)
    {
        for (size_t i{}; i < Y; ++i)
        {
            if (arg[i][j] == '|')
            {
                auto var = new side();
                pair.second.push_back(var);
                pair.second.back()->IsHoris = false;
                auto edge1 = std::find_if(pair.first.begin(), pair.first.end(), [i, j](corner const *el)
                                          {
                    std::pair<int,int> tempPair(i - 1,j);
                    return el->coords.first == tempPair.first && el->coords.second == tempPair.second; });
                pair.second.back()->edges.first = *edge1;
                (*edge1)->sides.push_back(pair.second.back());

                while (arg[i][j] != '+')
                {
                    ++i;
                }
                auto edge2 = std::find_if(pair.first.begin(), pair.first.end(), [i, j](corner const *el)
                                          {
                    std::pair<int,int> tempPair(i,j);
                    return el->coords.first == tempPair.first && el->coords.second == tempPair.second; });
                pair.second.back()->edges.second = *edge2;
                (*edge2)->sides.push_back(pair.second.back());
            }
        }
    }
    return pair;
}

std::vector<std::string> strArray(std::vector<std::string> &arg2)
{
    std::vector<std::string> arg = FillStringVector(arg2);
    auto cornersAndSides = Filter(arg);
    auto boundary = Boundary(arg, nextDirection);
    for (auto i : cornersAndSides.second)
    {
        i->IsBoundary = isBoundary(i, boundary);
    }
    std::vector<std::string> figureVec;
    std::vector<std::string> figure{arg};
    figure = BlankStringArray(figure);

    auto point = cornersAndSides.first[0];
    auto temp = point;
    while (true)
    {
        auto innerBoundary = Boundary(arg, ReverseNextDir);
        point = cornersAndSides.first[0];
        temp = point;
        auto linesToDraw = [&](std::vector<side *> sides)
        {
            std::vector<side *> sidesToDraw;
            side *previousLine{};
            point = temp;
            do
            {
                auto line = *(std::find_if(point->sides.begin(), point->sides.end(), [=](side *arg)
                                           { 
                    bool IsBoundary = isBoundary(arg, innerBoundary);
                    return IsBoundary && arg != previousLine; }));
                sidesToDraw.push_back(line);
                point = (line->edges.first != point) ? line->edges.first : line->edges.second;
                previousLine = line;
            } while (point != temp);
            return sidesToDraw;
        };
        auto lines = linesToDraw(cornersAndSides.second);

        side *previousLine{};
        for (auto line : lines)
        {
            line->DrawLineInStringVec(figure);
            if (needsFix(line->edges.first, line, previousLine))
                PlusToSomeOther(figure, line->edges.first, line);
            else if (needsFix(line->edges.second, line, previousLine))
                PlusToSomeOther(figure, line->edges.second, line);

            previousLine = line;
        }

        for (auto line : lines)
        {
            if (!line->IsBoundary)
            {
                line->IsBoundary = true;
            }
            else
            {
                auto edgeOne{line->edges.first};
                auto edgeTwo{line->edges.second};
                erase(edgeOne->sides, line);
                if (edgeOne->sides.empty())
                {
                    erase(cornersAndSides.first, edgeOne);
                    delete edgeOne;
                }
                erase(edgeTwo->sides, line);
                if (edgeTwo->sides.empty())
                {
                    erase(cornersAndSides.first, edgeTwo);
                    delete edgeTwo;
                }
                erase(cornersAndSides.second, line);
                delete line;
            }
        }
        if (cornersAndSides.first.empty())
            break;

        figure = FilterStringVec(figure);
        figureVec.push_back(StrVecToStr(figure));
        figure = BlankStringArray(arg);

        for (auto i : cornersAndSides.second)
        {
            arg = i->DrawLineInStringVec(arg);
        }
        boundary = Boundary(arg, nextDirection);

        point = cornersAndSides.first.front();
    }
    return figureVec;
}

std::vector<std::string> FilterStringVec(std::vector<std::string> &arg)
{
    std::vector<std::string> newArg = arg;
    for (size_t i{}; i < newArg.size(); ++i)
        while (newArg[i].back() == ' ')
            if (std::all_of(newArg[i].begin(), newArg[i].end(), [](char i)
                            { return i == ' '; }))
            {
                newArg.erase(newArg.begin() + i);
                if (i == newArg.size())
                    break;
            }
            else
                newArg[i].pop_back();
    auto bar = 0;
    auto AllStartBlank = [](std::vector<std::string> arg)
    {
        for (size_t i{}; i < arg.size(); ++i)
            if (!arg[i].find_first_not_of(' ') > 0)
                return false;
        return true;
    };
    while (AllStartBlank(newArg))
    {
        for (size_t i{}; i < newArg.size(); ++i)
            newArg[i].erase(newArg[i].begin());
    }
    return newArg;
}

void ReverseNextDir(std::vector<std::string> &arg, std::pair<int, int> previousPoint, size_t &x, size_t &y)
{
    auto dir = incomingDirection(previousPoint, x, y);
    bool hasPointsAbove{};
    if (y - 1 != std::string::npos)
    {
        hasPointsAbove = arg[y - 1][x] == '|' || arg[y - 1][x] == '+';
    }

    bool hasPointsToTheRight{};
    if (arg[y].length() - 1 >= x + 1)
    {
        hasPointsToTheRight = arg[y][x + 1] == '-' || arg[y][x + 1] == '+';
    }

    bool hasPointsBelow{};
    if (arg.size() - 1 >= y + 1)
    {
        if (arg[y + 1].length() >= x)
            hasPointsBelow = arg[y + 1][x] == '|' || arg[y + 1][x] == '+';
    }

    bool hasPointsToTheLeft{};
    if (x - 1 != std::string::npos)
    {
        hasPointsToTheLeft = arg[y][x - 1] == '-' || arg[y][x - 1] == '+';
    }
    switch (dir)
    {
    case IncomingDiredction::up:
    {
        if (arg[y][x] == '|')
        {
            ++y;
            break;
        }

        if (hasPointsToTheLeft)
            --x;
        else if (hasPointsBelow)
            ++y;
        else
            ++x;
        break;
    }
    case IncomingDiredction::down:
    {
        if (arg[y][x] == '|')
        {
            --y;
            break;
        }

        if (hasPointsToTheRight)
            ++x;
        else if (hasPointsAbove)
            --y;
        else
            --x;
        break;
    }
    case IncomingDiredction::left:
    {
        if (arg[y][x] == '-')
        {
            ++x;
            break;
        }
        if (hasPointsBelow)
            ++y;
        else if (hasPointsToTheRight)
            ++x;
        else
            --y;
        break;
    }
    case IncomingDiredction::right:
    {
        if (arg[y][x] == '-')
        {
            --x;
            break;
        }
        if (hasPointsAbove)
            --y;
        else if (hasPointsToTheLeft)
            --x;
        else
            ++y;
        break;
    }
    default:
        break;
    }
}

void nextDirection(std::vector<std::string> &arg, std::pair<int, int> previousPoint, size_t &x, size_t &y)
{
    auto dir = incomingDirection(previousPoint, x, y);
    bool hasPointsAbove{};
    if (y - 1 != std::string::npos)
    {
        hasPointsAbove = arg[y - 1][x] == '|' || arg[y - 1][x] == '+';
    }

    bool hasPointsToTheRight{};
    if (arg[y].length() - 1 >= x + 1)
    {
        hasPointsToTheRight = arg[y][x + 1] == '-' || arg[y][x + 1] == '+';
    }

    bool hasPointsBelow{};
    if (arg.size() - 1 >= y + 1)
    {
        if (arg[y + 1].length() >= x)
            hasPointsBelow = arg[y + 1][x] == '|' || arg[y + 1][x] == '+';
    }

    bool hasPointsToTheLeft{};
    if (x - 1 != std::string::npos)
    {
        hasPointsToTheLeft = arg[y][x - 1] == '-' || arg[y][x - 1] == '+';
    }
    switch (dir)
    {
    case IncomingDiredction::up:
    {
        if (arg[y][x] == '|')
        {
            ++y;
            break;
        }
        if (hasPointsToTheRight)
            ++x;
        else if (hasPointsBelow)
            ++y;
        else
            --x;
        break;
    }
    case IncomingDiredction::down:
    {
        if (arg[y][x] == '|')
        {
            --y;
            break;
        }
        if (hasPointsToTheLeft)
            --x;
        else if (hasPointsAbove)
            --y;
        else
            ++x;
        break;
    }
    case IncomingDiredction::left:
    {
        if (arg[y][x] == '-')
        {
            ++x;
            break;
        }
        if (hasPointsAbove)
            --y;
        else if (hasPointsToTheRight)
            ++x;
        else
            ++y;
        break;
    }
    case IncomingDiredction::right:
    {
        if (arg[y][x] == '-')
        {
            --x;
            break;
        }
        if (hasPointsBelow)
            ++y;
        else if (hasPointsToTheLeft)
            --x;
        else
            --y;
        break;
    }
    default:
        break;
    }
}

std::vector<std::pair<int, int>> Boundary(std::vector<std::string> &arg, std::function<void(std::vector<std::string> &, std::pair<int, int>, size_t &, size_t &)> nextMove, size_t x, size_t y, std::pair<int, int> startingCoord, std::pair<int, int> previousCoord)
{
    std::vector<std::pair<int, int>> BoundaryCoordinates;
    size_t i{};
    if (x == 0 && y == 0)
    {
        auto x_var = arg[i].find_first_not_of(' ');
        while (x_var == std::string::npos)
        {
            ++i;
            x_var = arg[i].find_first_not_of(' ');
        }
        startingCoord.first = i;
        startingCoord.second = x_var;
        y = i;
        x = x_var;
        previousCoord = {y, x};
        BoundaryCoordinates.push_back({y, x});
    }
    auto temp_x = x;
    auto temp_y = y;
    nextMove(arg, previousCoord, x, y);
    previousCoord = {temp_y, temp_x};
    BoundaryCoordinates.push_back({y, x});

    if ((BoundaryCoordinates.end() - 1)->first != startingCoord.first || (BoundaryCoordinates.end() - 1)->second != startingCoord.second)
    {
        auto temp2 = Boundary(arg, nextMove, x, y, startingCoord, previousCoord);
        BoundaryCoordinates.insert(BoundaryCoordinates.end(), temp2.begin(), temp2.end());
    }
    else
        BoundaryCoordinates.pop_back();
    return BoundaryCoordinates;
}

IncomingDiredction incomingDirection(std::pair<int, int> previousPoint, size_t x, size_t y)
{
    if (previousPoint.first < y)
        return IncomingDiredction::up;
    else if (previousPoint.second > x)
        return IncomingDiredction::right;
    else if (previousPoint.second < x)
        return IncomingDiredction::left;
    else
        return IncomingDiredction::down;
}

std::vector<std::string> &BlankStringArray(std::vector<std::string> &vecArr)
{
    for (size_t i{}, j{}; i < vecArr.size(); ++i)
    {
        std::replace_if(
            vecArr[i].begin(), vecArr[i].end(), [](char dummyChar)
            { return true; },
            ' ');
    }
    return vecArr;
}

std::string StrVecToStr(std::vector<std::string> &strVec)
{
    std::string str;
    for (auto i : strVec)
    {
        str += i;
        str.push_back('\n');
    }
    str.pop_back();
    return str;
}

bool needsFix(corner *corner, side *side1, side *side2)
{
    if (!side1 || !side2 || side1 == side2)
        return false;
    if (side1->IsHoris == side2->IsHoris)
    {
        bool hasSide1{std::any_of(corner->sides.begin(), corner->sides.end(), [side1](auto el)
                                  { return el == side1; })};
        bool hasSide2{std::any_of(corner->sides.begin(), corner->sides.end(), [side2](auto el)
                                  { return el == side2; })};
        return hasSide1 && hasSide2;
    }

    return false;
}

void PlusToSomeOther(std::vector<std::string> &arg, corner *corner, side *AssociatedSide)
{
    if (AssociatedSide->IsHoris)
        arg[corner->coords.first][corner->coords.second] = '-';
    else
        arg[corner->coords.first][corner->coords.second] = '|';
}
