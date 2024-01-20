#ifndef functions_H
#define functions_H
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include "corner.h"
#include "side.h"
#include "IncomingDirection.h"

template <class T>
int erase(std::vector<T> &c, T value);

void PlusToSomeOther(std::vector<std::string> &, corner *, side *);

bool needsFix(corner *, side *, side *);

std::vector<std::string> &BlankStringArray(std::vector<std::string> &vecArr);

std::vector<std::string> FilterStringVec(std::vector<std::string> &);

IncomingDiredction incomingDirection(std::pair<int, int>, size_t, size_t);

std::string StrVecToStr(std::vector<std::string> &strVec);

void nextDirection(std::vector<std::string> &, std::pair<int, int>, size_t &, size_t &);

void ReverseNextDir(std::vector<std::string> &, std::pair<int, int>, size_t &, size_t &);

std::vector<std::string> FillStringVector(std::vector<std::string> &strArr2);

std::vector<std::pair<int, int>> Boundary(std::vector<std::string> &, std::function<void(std::vector<std::string> &, std::pair<int, int>, size_t &, size_t &)> pred, size_t x = 0, size_t y = 0, std::pair<int, int> startingCoord = {0, 0}, std::pair<int, int> previousCoord = {});

bool isBoundary(side *line, std::vector<std::pair<int, int>> BoundaryCoordinates);

std::pair<std::vector<corner *>, std::vector<side *>> Filter(std::vector<std::string> &arg2);

std::vector<std::string> strArray(std::vector<std::string> &arg2);


#endif