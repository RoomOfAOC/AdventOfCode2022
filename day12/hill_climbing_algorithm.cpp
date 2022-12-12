//
// Created by Harold on 2022/12/12.
//

#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <deque>
#include <iostream>

#include "mat.h"

size_t part_1(char const* fn)
{
    std::ifstream ifs(fn);
    Mat<int> mat;
    read_all(ifs, mat, 'a');
    Point start{}, end{};
    std::unordered_set<Point, Point::PointHasher> visited;
    std::deque<std::pair<Point, int>> paths;
    std::vector<Point> neighbors;
    neighbors.reserve(8);

    for (auto i = 0; i < mat.rows; i++)
        for (auto j = 0; j < mat.cols; j++)
            if (mat(i, j) == 'S' - 'a')
            {
                start = {i, j};
                mat(i, j) = 0;
            }
            else if (mat(i, j) == 'E' - 'a')
            {
                end = {i, j};
                mat(i, j) = 26;
            }
    //std::cout << start << ", " << end << '\n';

    visited.insert(start);
    paths.emplace_back(start, 0);
    int current_height = 0;
    while (!paths.empty())
    {
        auto [current, steps] = paths.front();
        //std::cout << current << ", " << steps << '\n';
        paths.pop_front();
        if (current == end)
            return steps;
        current_height = mat(current);
        neighbors.clear();
        mat.find_4_neighbors(current, neighbors);
        for (auto const& p : neighbors)
        {
            if (visited.find(p) != visited.end())
                continue;
            if (mat(p) - current_height > 1)
                continue;
            visited.insert(p);
            paths.emplace_back(p, steps + 1);
        }  
    }

    return 0;
}

size_t part_2(char const* fn)
{
    std::ifstream ifs(fn);
    Mat<int> mat;
    read_all(ifs, mat, 'a');
    Point start{}, end{};
    std::unordered_set<Point, Point::PointHasher> visited;
    std::deque<std::pair<Point, int>> paths;
    std::vector<Point> neighbors;
    neighbors.reserve(8);

    for (auto i = 0; i < mat.rows; i++)
        for (auto j = 0; j < mat.cols; j++)
            if (mat(i, j) == 'S' - 'a')
            {
                start = {i, j};
                mat(i, j) = 0;
            }
            else if (mat(i, j) == 'E' - 'a')
            {
                end = {i, j};
                mat(i, j) = 26;
            }
            else if (mat(i, j) == 0)
            {
                visited.insert(Point(i, j));
                paths.emplace_back(Point(i, j), 0);
            }

    visited.insert(start);
    paths.emplace_back(start, 0);
    int current_height = 0;
    while (!paths.empty())
    {
        auto [current, steps] = paths.front();
        paths.pop_front();
        if (current == end)
            return steps;
        current_height = mat(current);
        neighbors.clear();
        mat.find_4_neighbors(current, neighbors);
        for (auto const& p : neighbors)
        {
            if (visited.find(p) != visited.end())
                continue;
            if (mat(p) - current_height > 1)
                continue;
            visited.insert(p);
            paths.emplace_back(p, steps + 1);
        }  
    }

    return 0;
}

int main(int argc, char* argv[])
{
    
    auto p = part_1(argv[1]);
    std::cout << "What is the fewest steps required to move from your current position to the location that should get the best signal?" << '\n'
              << p << std::endl;

    auto s = part_2(argv[1]);
    std::cout << "What is the fewest steps required to move starting from any square with elevation a to the location that should get the best signal?" << '\n'
              << s << std::endl;

    return 0;
}