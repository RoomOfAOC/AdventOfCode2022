//
// Created by Harold on 2022/12/2.
//

#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iostream>

int main(int argc, char* argv[])
{
    std::ifstream ifs(argv[1]);
    std::string line;
    unsigned long long cnt = 0;
    std::vector<unsigned long long> elf;
    while (std::getline(ifs, line))
    {
        if (!line.empty())
            cnt += std::stoull(line);
        else
        {
            elf.push_back(cnt);
            cnt = 0;
        }
    }

    auto max = *std::max_element(elf.begin(), elf.end());
    std::cout << "How many total Calories is that Elf carrying?" << '\n'
              << max << std::endl;

    std::partial_sort(elf.begin(), elf.begin() + 3, elf.end(), 
        [](unsigned long long x, unsigned long long y){ return x > y; });
    auto total = std::reduce(elf.begin(), elf.begin() + 3);
    std::cout << "How many Calories are those Elves carrying in total?" << '\n'
              << total << std::endl;

    return 0;
}