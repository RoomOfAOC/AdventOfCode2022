//
// Created by Harold on 2022/12/23.
//

#include <fstream>
#include <string>
#include <array>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <iostream>

#include "../day12/mat.h"

using ElvesSet = std::unordered_set<Point, Point::PointHasher>;

std::ostream& operator<<(std::ostream& os, ElvesSet const& es)
{
    for (auto const& e : es)
        os << e << " ";
    return os;
}

ElvesSet parse_input(char const* fn)
{
    ElvesSet elves_pos;
    std::ifstream ifs(fn);
    std::string line;
    int x = 0, y = 0;
    while (std::getline(ifs, line))
    {
        x = 0;
        for (auto c : line)
        {
            if (c == '#')
                elves_pos.insert(Point{x, y});
            x++;
        }
        y++;
    }

    return elves_pos;
}

std::array<Point, 8> get_8_neighbors(Point const& p)
{
    return {
        Point{p.x - 1, p.y - 1},
        {p.x, p.y - 1},
        {p.x + 1, p.y - 1},
        {p.x + 1, p.y},
        {p.x + 1, p.y + 1},
        {p.x, p.y + 1},
        {p.x - 1, p.y + 1},
        {p.x - 1, p.y}};
}

void simulate_one_round(ElvesSet& elves_pos, int round)
{
    std::unordered_map<Point, std::pair<Point, int>, Point::PointHasher> move_counter;
    for (auto const& pos : elves_pos)
    {
        bool is_move = false;
        // check neighbors
        auto neighbors = get_8_neighbors(pos);
        for (auto const& p : neighbors)
            if (elves_pos.find(p) != elves_pos.end())
            {
                is_move = true;
                break;
            }
        // N S W E
        std::array<std::array<Point, 3>, 4> points{
            std::array<Point, 3>{neighbors[0], neighbors[1], neighbors[2]},
            {neighbors[4], neighbors[5], neighbors[6]},
            {neighbors[0], neighbors[7], neighbors[6]},
            {neighbors[2], neighbors[3], neighbors[4]},
        };
        if (is_move)
        {
            for (auto i = 0; i < 4; i++)
            {
                auto moving_dir = (round + i) % 4;
                auto check_points = points[moving_dir];
                if (std::all_of(check_points.begin(), check_points.end(), [&elves_pos](Point const& p){ return elves_pos.find(p) == elves_pos.end(); }))
                {
                    auto const& new_pos = check_points[1];
                    if (move_counter.find(new_pos) == move_counter.end())
                        move_counter[new_pos] = {pos, 1};
                    else
                        move_counter[new_pos].second++;
                    break;
                }
            }
        }
    }
    for (auto const& [k, v] : move_counter)
        if (v.second == 1)
        {
            elves_pos.erase(v.first);
            elves_pos.insert(k);
        }
}

int part_1(char const* fn)
{
    auto elves_pos = parse_input(fn);
    for (auto i = 0; i < 10; i++)
        simulate_one_round(elves_pos, i);
    auto x_min_max = std::minmax_element(elves_pos.cbegin(), elves_pos.cend(), [](Point const& p1, Point const& p2){ return p1.x < p2.x; });
    auto y_min_max = std::minmax_element(elves_pos.cbegin(), elves_pos.cend(), [](Point const& p1, Point const& p2){ return p1.y < p2.y; });
    int cnt = 0;
    for (auto y = y_min_max.first->y; y <= y_min_max.second->y; y++)
        for (auto x = x_min_max.first->x; x <= x_min_max.second->x; x++)
            if (elves_pos.find(Point{x, y}) == elves_pos.end())
                cnt++;
    return cnt;
}

int part_2(char const* fn)
{
    auto elves_pos = parse_input(fn);
    ElvesSet cpy;
    for (auto i = 0; ; i++)
    {
        cpy = elves_pos;
        simulate_one_round(elves_pos, i);
        if (cpy == elves_pos)
            return i + 1;
    }
    
    return 0;
}


int main(int argc, char* argv[])
{
    
    auto p = part_1(argv[1]);
    std::cout << "How many empty ground tiles does that rectangle contain?" << '\n'
              << p << std::endl;

    auto s = part_2(argv[1]);
    std::cout << "What is the number of the first round where no Elf moves?" << '\n'
              << s << std::endl;

    return 0;
}