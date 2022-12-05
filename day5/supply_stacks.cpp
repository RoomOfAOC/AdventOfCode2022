//
// Created by Harold on 2022/12/5.
//

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <iostream>

std::string part_1(char const* fn)
{
    std::string res;
    std::ifstream ifs(fn);
    std::string line;
    std::vector<std::string> initial_stacks;
    bool initial_stacks_read = false;
    std::vector<std::string> stacks;
    int moves{}, from{}, to{};
    while (std::getline(ifs, line))
    {
        // parse initial stacks
        if (line.empty())
            initial_stacks_read = true;
        if (!initial_stacks_read)
        {
            initial_stacks.push_back(line);
            continue;
        }
        // handle inital stacks
        if (line.empty())
        {
            std::vector<int> positions;
            std::string const& num = initial_stacks.back();
            for (auto i = 0; i < num.size(); i++)
                if (std::isalnum(num[i]))
                    positions.push_back(i);
            stacks.resize(positions.size());
            for (auto i = 0; i < initial_stacks.size() - 1; i++)
                for (auto j = 0; j < positions.size(); j++)
                {
                    auto c = initial_stacks[i][positions[j]];
                    if (std::isupper(c))
                        stacks[j].push_back(c);
                }
            for (auto& s : stacks)
                std::reverse(s.begin(), s.end());
            continue;
        }
        // handle movements
        std::sscanf(line.data(), "move %d from %d to %d", &moves, &from, &to);
        from--;
        to--;
        for (auto m = 0; m < moves; m++)
        {
            auto c = stacks[from].back();
            stacks[from].pop_back();
            stacks[to].push_back(c);
        }
    }
    for (auto const s : stacks)
        res.push_back(s.back());
    return res;
}

std::string part_2(char const* fn)
{
    std::string res;
    std::ifstream ifs(fn);
    std::string line;
    std::vector<std::string> initial_stacks;
    bool initial_stacks_read = false;
    std::vector<std::string> stacks;
    int moves{}, from{}, to{};
    size_t offset = 0;
    while (std::getline(ifs, line))
    {
        // parse initial stacks
        if (line.empty())
            initial_stacks_read = true;
        if (!initial_stacks_read)
        {
            initial_stacks.push_back(line);
            continue;
        }
        // handle inital stacks
        if (line.empty())
        {
            std::vector<int> positions;
            std::string const& num = initial_stacks.back();
            for (auto i = 0; i < num.size(); i++)
                if (std::isalnum(num[i]))
                    positions.push_back(i);
            stacks.resize(positions.size());
            for (auto i = 0; i < initial_stacks.size() - 1; i++)
                for (auto j = 0; j < positions.size(); j++)
                {
                    auto c = initial_stacks[i][positions[j]];
                    if (std::isupper(c))
                        stacks[j].push_back(c);
                }
            for (auto& s : stacks)
                std::reverse(s.begin(), s.end());
            continue;
        }
        // handle movements
        std::sscanf(line.data(), "move %d from %d to %d", &moves, &from, &to);
        from--;
        to--;
        offset = stacks[from].size() - moves;
        stacks[to] += stacks[from].substr(offset);
        stacks[from].erase(offset);
    }
    for (auto const s : stacks)
        res.push_back(s.back());
    return res;
}

int main(int argc, char* argv[])
{
    
    auto p = part_1(argv[1]);
    std::cout << "After the rearrangement procedure completes, what crate ends up on top of each stack?" << '\n'
              << p << std::endl;

    p = part_2(argv[1]);
    std::cout << "After the rearrangement procedure completes, what crate ends up on top of each stack?" << '\n'
              << p << std::endl;

    return 0;
}