//
// Created by Harold on 2022/12/18.
//

#include <fstream>
#include <string>
#include <cstdio>
#include <array>
#include <unordered_set>
#include <deque>
#include <iostream>

#include "../day12/mat.h"

using Point3dSet = std::unordered_set<Point3d, Point3d::Hasher>;

Point3dSet parse_input(char const* fn)
{
    Point3dSet points;
    std::ifstream ifs(fn);
    std::string line;
    int x = 0, y = 0, z = 0;
    while (std::getline(ifs, line))
    {
        std::sscanf(line.c_str(), "%d,%d,%d", &x, &y, &z);
        points.emplace(x, y, z);
    }
    return points;
}

std::array<Point3d, 6> get_6_neighbors(Point3d const& p)
{
    return {Point3d{p.x - 1, p.y, p.z}, {p.x + 1, p.y, p.z}, {p.x, p.y - 1, p.z}, {p.x, p.y + 1, p.z}, {p.x, p.y, p.z - 1}, {p.x, p.y, p.z + 1}};
}

int part_1(char const* fn)
{
    auto points = parse_input(fn);
    int area = 0;
    for (auto const& p : points)
        for (auto const& n : get_6_neighbors(p))
            if (points.find(n) == points.end())
                area++;
    return area;
}

// x_min, x_max, y_min, y_max, z_min, z_max
std::array<int, 6> get_bounds(Point3dSet points)
{
    auto min = std::numeric_limits<int>::min();
    auto max = std::numeric_limits<int>::max();
    std::array<int, 6> bounds{max, min, max, min, max, min};
    for (auto const& p : points)
    {
        if (p.x < bounds[0])
            bounds[0] = p.x;
        if (p.x > bounds[1])
            bounds[1] = p.x;
        if (p.y < bounds[2])
            bounds[2] = p.y;
        if (p.y > bounds[3])
            bounds[3] = p.y;
        if (p.z < bounds[4])
            bounds[4] = p.z;
        if (p.z > bounds[5])
            bounds[5] = p.z;
    }
    return bounds;
}

bool expand(Point3d const& p, Point3dSet points)
{
    auto bounds = get_bounds(points);
    Point3dSet visited;
    std::deque<Point3d> expanding_q;
    expanding_q.push_back(p);
    while (!expanding_q.empty())
    {
        auto curr_p = expanding_q.front();
        expanding_q.pop_front();
        // this simple assertion also works
        // if (visited.size() >= points.size())
        //     return false;
        // out of bounds
        if (curr_p.x < bounds[0] || curr_p.x > bounds[1] || curr_p.y < bounds[2] || curr_p.y > bounds[3] || curr_p.z < bounds[4] || curr_p.z > bounds[5])
            return false;
        if (auto [_, yes] = visited.insert(curr_p); yes)
        {
            for (auto const& n : get_6_neighbors(curr_p))
                if (visited.find(n) == visited.end() && points.find(n) == points.end())
                    expanding_q.push_back(n);
        }
    }
    return true;
}

int part_2(char const* fn)
{
    auto points = parse_input(fn);
    int area = 0;
    for (auto const& p : points)
    {
        for (auto const& n : get_6_neighbors(p))
            if (points.find(n) == points.end() && !expand(n, points))
                area++;
    }
            
    return area;
}


int main(int argc, char* argv[])
{
    
    auto p = part_1(argv[1]);
    std::cout << "What is the surface area of your scanned lava droplet?" << '\n'
              << p << std::endl;

    auto s = part_2(argv[1]);
    std::cout << "What is the exterior surface area of your scanned lava droplet?" << '\n'
              << s << std::endl;

    return 0;
}