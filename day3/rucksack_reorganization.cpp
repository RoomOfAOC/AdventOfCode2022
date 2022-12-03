//
// Created by Harold on 2022/12/3.
//

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

size_t part_1(char const* fn)
{
    int occurrences[52]{0};

    std::ifstream ifs(fn);
    std::string line;
    size_t p = 0;
    int pos;
    while (std::getline(ifs, line))
    {
        auto N = line.size();
        for (auto i = 0; i < N / 2; ++i)
        {
            char c = line[i];
            pos = std::islower(c) ? c - 'a' : c - 'A' + 26;
            occurrences[pos] = 1;
        }
        for (auto i = N / 2; i < N; ++i)
        {
            char c = line[i];
            pos = std::islower(c) ? c - 'a' : c - 'A' + 26;
            if (occurrences[pos] == 1)
            {
                p += pos + 1;
                break;
            }
        }
        memset(&occurrences, 0, sizeof(int) * 52);
    }
    return p;
}

size_t part_2(char const* fn)
{
    int occurrences[52]{0};

    std::ifstream ifs(fn);
    std::vector<std::string> lines;
    std::string line;
    size_t p = 0;
    int pos;
    while (std::getline(ifs, line))
        lines.push_back(line);
    for (auto i = 0; i < lines.size(); i += 3)
    {
        for (auto c : lines[i])
            occurrences[std::islower(c) ? c - 'a' : c - 'A' + 26] = 1;
        for (auto c : lines[i + 1])
            if (occurrences[std::islower(c) ? c - 'a' : c - 'A' + 26] == 1)
                occurrences[std::islower(c) ? c - 'a' : c - 'A' + 26] = 2;
        for (auto c : lines[i + 2])
        {
            pos = std::islower(c) ? c - 'a' : c - 'A' + 26;
            if (occurrences[pos] == 2)
            {
                p += pos + 1;
                break;
            }
        }
        memset(&occurrences, 0, sizeof(int) * 52);
    }
    return p;
}

int main(int argc, char* argv[])
{
    
    auto p = part_1(argv[1]);
    std::cout << "What is the sum of the priorities of those item types?" << '\n'
              << p << std::endl;

    p = part_2(argv[1]);
    std::cout << "What is the sum of the priorities of those item types?" << '\n'
              << p << std::endl;

    return 0;
}