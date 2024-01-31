#include <iostream>
#include <algorithm>
#include <functional>
#include "corner.h"
#include "side.h"
#include "IncomingDirection.h"
#include "Functions.h"

int main()
{
    std::vector<std::string> shape{"+-----+  +---+        +-------+",
                                   "|     |  |   |        |       |",
                                   "|     |  |   +-----+  |   +---+",
                                   "|     |  |   |     |  |   |",
                                   "|     +--+   |     +--+   +---+",
                                   "|        |   |                |",
                                   "|     +--+   |  +-+       +---+",
                                   "|     |      |  | |       |",
                                   "+-----+--+---+--+ +-------+",
                                   "|     |  |   |            |",
                                   "|     |  |   |   +--+     |",
                                   "|     |  |   |   |  |     |",
                                   "+-----+--+---+---+  +-----+"};
    // std::vector<std::string> shape{"+-----+  +---+",
    //                                "|     |  |   |",
    //                                "|     |  |   +-----+",
    //                                "|     |  |         |",
    //                                "|     +--+         |",
    //                                "|        |         |",
    //                                "|     +--+   +-----+",
    //                                "|     |      |",
    //                                "+-----+--+---+",
    //                                "|     |  |   |",
    //                                "|     |  |   |",
    //                                "|     |  |   |",
    //                                "+-----+--+---+"};
        // std::vector<std::string> shape {"+-----+  +---+",
        //                                 "|     |  |   |",
        //                                 "|     |  |   +-----+",
        //                                 "|     |  |         |",
        //                                 "|     +--+         |",
        //                                 "|        |         |",
        //                                 "|     +--+   +-----+",
        //                                 "|     |      |",
        //                                 "+-----+--+---+"};
        // std::vector<std::string> shape {"+-----+",
        //                                 "|     |",
        //                                 "|     |",
        //                                 "+-----+"};
    auto strArr = BreakIntoPieces(shape);
    for (auto i : strArr)
    {
        std::cout << i << std::endl;
    }
}