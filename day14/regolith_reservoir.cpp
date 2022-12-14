//
// Created by Harold on 2022/12/14.
//

#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <unordered_set>
#include <algorithm>
#include <iostream>

#include "../day12/mat.h"

using u_set = std::unordered_set<Point, Point::PointHasher>;

std::vector<Point> parse_points(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    std::regex reg(" -> ");
    std::vector<Point> points, rock_points;
    while (std::getline(ifs, line))
    {
        rock_points.clear();
        std::sregex_token_iterator it(line.begin(), line.end(), reg, -1);
        decltype(it) end;
        for (; it != end; ++it)
        {
            auto s = it->str();
            auto comma_pos = s.find(',');
            rock_points.emplace_back(std::stoi(s.substr(0, comma_pos)), std::stoi(s.substr(comma_pos + 1)));
        }
        for (auto i = 1; i < rock_points.size(); i++)
        {
            auto ps = rock_points[i - 1];
            auto pe = rock_points[i];
            points.emplace_back(ps);
            if (ps.x != pe.x)
            {
                auto s = ps.x;
                auto e = pe.x;
                if (s > e) std::swap(s, e);
                for (auto i = s + 1; i < e; i++)
                    points.emplace_back(i, ps.y);
            }
            else if (ps.y != pe.y)
            {
                auto s = ps.y;
                auto e = pe.y;
                if (s > e) std::swap(s, e);
                for (auto i = s + 1; i < e; i++)
                    points.emplace_back(ps.x, i);
            }
        }
        points.push_back(rock_points[rock_points.size() - 1]);
    }

    // auto x_min_max = std::minmax_element(points.begin(), points.end(), [](Point const& p1, Point const& p2){ return p1.x < p2.x; });
    // auto x_min = x_min_max.first->x;
    // auto x_max = x_min_max.second->x;
    // auto y_min_max = std::minmax_element(points.begin(), points.end(), [](Point const& p1, Point const& p2){ return p1.y < p2.y; });
    // auto y_min = y_min_max.first->y;
    // auto y_max = y_min_max.second->y;

    // points.emplace_back(500, 0);
    // auto rows = y_max - y_min + 1;
    // auto cols = x_max - x_min + 1;
    // Mat<int> map(rows, cols);
    // for (auto const& p : points)
    //     map(p.y - y_min, p.x - x_min) = p.x == 500 && p.y == 0 ? 2 : 1;
    // std::cout << map << '\n';

    return points;
}

size_t part_1(char const* fn)
{
    auto points = parse_points(fn);
    u_set visited_points(points.begin(), points.end());
    auto y_max = std::minmax_element(points.begin(), points.end(), [](Point const& p1, Point const& p2){ return p1.y < p2.y; }).second->y;

    auto come_to_rest = [&visited_points, y_max](Point const& current_point, auto&& come_to_rest)
    {
        Point new_point = current_point;
        // down
        while (visited_points.find(new_point) == visited_points.end() && new_point.y <= y_max)
            new_point.y++;
        if (new_point.y > y_max)
            return false;
        // left
        new_point.x--;
        if (visited_points.find(new_point) == visited_points.end())
            return come_to_rest(new_point, come_to_rest);
        // right
        new_point.x += 2;
        if (visited_points.find(new_point) == visited_points.end())
            return come_to_rest(new_point, come_to_rest);
        // up
        new_point.x--;
        new_point.y--;
        visited_points.insert(new_point);
        return true;
    };

    Point sand{500, 0};
    size_t cnt = 0;
    while (come_to_rest(sand, come_to_rest))
        cnt++;
    return cnt;
}

size_t part_2(char const* fn)
{
    auto points = parse_points(fn);
    u_set visited_points(points.begin(), points.end());
    auto y_max = std::minmax_element(points.begin(), points.end(), [](Point const& p1, Point const& p2){ return p1.y < p2.y; }).second->y;
    y_max += 2;

    auto come_to_rest = [&visited_points, y_max](Point const& current_point, auto&& come_to_rest)
    {
        // can't beyond the sand entrance
        if (visited_points.find(Point{500, 1}) != visited_points.end() &&
            visited_points.find(Point{499, 1}) != visited_points.end() &&
            visited_points.find(Point{501, 1}) != visited_points.end())
            return false;

        Point new_point = current_point;
        // down
        while (visited_points.find(new_point) == visited_points.end() && new_point.y < y_max)
            new_point.y++;
        if (new_point.y == y_max)
        {
            new_point.y--;
            visited_points.insert(new_point);
            return true;
        }
        // left
        new_point.x--;
        if (visited_points.find(new_point) == visited_points.end())
            return come_to_rest(new_point, come_to_rest);
        // right
        new_point.x += 2;
        if (visited_points.find(new_point) == visited_points.end())
            return come_to_rest(new_point, come_to_rest);
        // up
        new_point.x--;
        new_point.y--;
        visited_points.insert(new_point);
        return true;
    };

    Point sand{500, 0};
    size_t cnt = 0;
    while (come_to_rest(sand, come_to_rest))
        cnt++;
    return ++cnt;
}


int main(int argc, char* argv[])
{
    
    auto p = part_1(argv[1]);
    std::cout << "How many units of sand come to rest before sand starts flowing into the abyss below?" << '\n'
              << p << std::endl;

    auto s = part_2(argv[1]);
    std::cout << "How many units of sand come to rest?" << '\n'
              << s << std::endl;

    return 0;
}