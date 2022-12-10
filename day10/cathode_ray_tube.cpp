//
// Created by Harold on 2022/12/10.
//

#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <iostream>

size_t part_1(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    std::map<int, int> cycles = {{20, 0}, {60, 0}, {100, 0}, {140, 0}, {180, 0}, {220, 0}};
    int cycle = 1;
    int x = 1;
    int sum = 0;
    while (std::getline(ifs, line))
    {
        cycle++;
        if (cycles.find(cycle) != cycles.end())
            cycles[cycle] = x;
        if (line[0] == 'n')
            continue;
        x += std::stoi(line.substr(5));
        cycle++;
        if (cycles.find(cycle) != cycles.end())
            cycles[cycle] = x;
    }

    for (auto const& [k, v] : cycles)
        sum += k * v;

    return sum;
}

std::string part_2(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    int cycle = 0; // [0, 39]
    int x = 1;
    std::stringstream ss;
    auto draw = [&ss](int x, int cycle)
    {
        cycle %= 40;
        // If the sprite is positioned such that one of its three pixels is the pixel currently being drawn, 
        // the screen produces a lit pixel (#); otherwise, the screen leaves the pixel dark (.).
        if (cycle == x - 1 || cycle == x || cycle == x + 1)
            ss << '#';
        else
            ss << '.';
    };
    auto is_change_line = [&ss](int cycle)
    {
        if (cycle % 40 == 0)
            ss << '\n';
    };
    while (std::getline(ifs, line))
    {
        draw(x, cycle);
        cycle++;
        is_change_line(cycle);
        if (line[0] == 'a')
        {
            draw(x, cycle);
            x += std::stoi(line.substr(5));
            cycle++;
            is_change_line(cycle);
        }
    }

    return ss.str();
}

int main(int argc, char* argv[])
{
    
    auto p = part_1(argv[1]);
    std::cout << "What is the sum of these six signal strengths?" << '\n'
              << p << std::endl;

    auto s = part_2(argv[1]);
    std::cout << "What eight capital letters appear on your CRT?" << '\n'
              << s << std::endl;


    // ####..##..#....#..#.###..#....####...##.
    // #....#..#.#....#..#.#..#.#....#.......#.
    // ###..#....#....####.###..#....###.....#.
    // #....#.##.#....#..#.#..#.#....#.......#.
    // #....#..#.#....#..#.#..#.#....#....#..#.
    // ####..###.####.#..#.###..####.#.....##..

    // EGLHBLFJ

    return 0;
}