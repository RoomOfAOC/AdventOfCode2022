//
// Created by Harold on 2022/12/6.
//

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iostream>

size_t part_1(char const* fn)
{
    std::string res;
    std::ifstream ifs(fn);
    std::string line;
    std::getline(ifs, line);
    for (size_t i = 0; i < line.size() - 4; i++)
    {
        auto seq = line.substr(i, 4);
        std::sort(seq.begin(), seq.end());
        if (std::unique(seq.begin(), seq.end()) == seq.end())
            return i + 4;
    }
    return 0;
}

size_t part_2(char const* fn)
{
    std::string res;
    std::ifstream ifs(fn);
    std::string line;
    std::getline(ifs, line);
    for (size_t i = 0; i < line.size() - 14; i++)
    {
        auto seq = line.substr(i, 14);
        std::sort(seq.begin(), seq.end());
        if (std::unique(seq.begin(), seq.end()) == seq.end())
            return i + 14;
    }
    return 0;
}

int main(int argc, char* argv[])
{
    
    auto p = part_1(argv[1]);
    std::cout << "How many characters need to be processed before the first start-of-packet marker is detected?" << '\n'
              << p << std::endl;

    p = part_2(argv[1]);
    std::cout << "How many characters need to be processed before the first start-of-message marker is detected?" << '\n'
              << p << std::endl;

    return 0;
}