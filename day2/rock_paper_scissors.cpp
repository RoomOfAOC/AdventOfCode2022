//
// Created by Harold on 2022/12/2.
//

#include <fstream>
#include <string>
#include <map>
#include <numeric>
#include <iostream>

unsigned long part_1(char const* fn)
{
    static std::map<char, unsigned> mapping
    {
        { 'A', 1 },
        { 'X', 1 },
        { 'B', 2 },
        { 'Y', 2 },
        { 'C', 3 },
        { 'Z', 3 }
    };
    constexpr unsigned win = 6;
    constexpr unsigned draw = 3;
    constexpr unsigned lose = 0;

    std::ifstream ifs(fn);
    std::string line;
    unsigned long scores = 0;
    while (std::getline(ifs, line))
    {
        auto op = mapping[line[0]];
        auto on = mapping[line[2]];
        scores += on;
        if (on == op) scores += draw;
        else if ((on - 1) % 3 == op % 3) scores += win;  // [1, 2] [2, 3] [3, 1]
        else scores += lose;
    }
    return scores;
}

unsigned long part_2(char const* fn)
{
    constexpr unsigned win = 6;
    constexpr unsigned draw = 3;
    constexpr unsigned lose = 0;

    std::ifstream ifs(fn);
    std::string line;
    unsigned long scores = 0;
    while (std::getline(ifs, line))
    {
        auto op = line[0] - 'A' + 1;
        auto on = line[2];
        // lose [1, 3] [2, 1] [3, 2]
        if (on == 'X')
        {
            scores += lose;
            scores += op == 1 ? 3 : op == 2 ? 1 : 2;
        }
        // draw
        else if (on == 'Y')
        {
            scores += draw;
            scores += op;
        }
        // win [1, 2] [2, 3] [3, 1]
        else
        {
            scores += win;
            scores += op == 1 ? 2 : op == 2 ? 3 : 1;
        }
    }
    return scores;
}

int main(int argc, char* argv[])
{
    
    auto scores = part_1(argv[1]);
    std::cout << "What would your total score be if everything goes exactly according to your strategy guide?" << '\n'
              << scores << std::endl;

    scores = part_2(argv[1]);
    std::cout << "What would your total score be if everything goes exactly according to your strategy guide?" << '\n'
              << scores << std::endl;

    return 0;
}