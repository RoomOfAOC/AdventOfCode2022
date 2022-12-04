//
// Created by Harold on 2022/12/4.
//

#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <iostream>

unsigned part_1(char const* fn)
{
    unsigned pairs = 0;
    std::ifstream ifs(fn);
    std::string line;
    int x1{}, x2{}, y1{}, y2{};
    while (std::getline(ifs, line))
    {
        std::sscanf(line.data(), "%d-%d,%d-%d", &x1, &x2, &y1, &y2);
        if ((x1 >= y1 && x2 <= y2) || (x1 <= y1 && x2 >= y2))
            pairs++;
    }
    return pairs;
}

unsigned part_2(char const* fn)
{
    unsigned pairs = 0;
    std::ifstream ifs(fn);
    std::string line;
    int x1{}, x2{}, y1{}, y2{};
    while (std::getline(ifs, line))
    {
        sscanf(line.data(), "%d-%d,%d-%d", &x1, &x2, &y1, &y2);
        if ((x1 <= y2 && x2 >= y1) || (y1 <= x2 && y2 >= x1))
            pairs++;
    }
    return pairs;
}

int main(int argc, char* argv[])
{
    
    auto p = part_1(argv[1]);
    std::cout << "In how many assignment pairs does one range fully contain the other?" << '\n'
              << p << std::endl;

    p = part_2(argv[1]);
    std::cout << "In how many assignment pairs do the ranges overlap?" << '\n'
              << p << std::endl;

    return 0;
}