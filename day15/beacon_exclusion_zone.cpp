//
// Created by Harold on 2022/12/15.
//

#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <iostream>

#include "../day12/mat.h"

using u_set = std::unordered_set<Point, Point::PointHasher>;

struct Sensor
{
    Point position;
    Point closest_beacon_position;
    unsigned search_distance = 0;
};

unsigned get_dist(int x1, int x2, int y1, int y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}

unsigned get_dist(Point const& p1, Point const& p2)
{
    return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

bool in_sensor_search_range(Sensor const& s, Point const& p)
{
    return get_dist(s.position, p) <= s.search_distance;
}

void parse_sensors(char const* fn, std::vector<Sensor>& sensors, int& x_min, int& x_max, int& y_min, int& y_max)
{
    std::ifstream ifs(fn);
    std::string line;
    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    while (std::getline(ifs, line))
    {
        if (4 == std::sscanf(line.c_str(), "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", &x1, &y1, &x2, &y2))
        {
            sensors.push_back({{x1, y1}, {x2, y2}, get_dist(x1, x2, y1, y2)});
            x_min = std::min({x_min, x1, x2});
            x_max = std::max({x_max, x2, x2});
            y_min = std::min({y_min, y1, y2});
            y_max = std::max({y_max, y1, y2});
        }
        else
            std::cerr << "parse error at line: " << line << '\n';
    }

    // auto rows = y_max - y_min + 1;
    // auto cols = x_max - x_min + 1;
    // Mat<char> map(rows, cols, '.');
    // for (auto const& s : sensors)
    // {
    //     map(s.position.y - y_min, s.position.x - x_min) = 'S';
    //     map(s.closest_beacon_position.y - y_min, s.closest_beacon_position.x - x_min) = 'B';
    // }
    // std::cout << map << '\n';
}

long part_1(char const* fn, int search_row)
{
    std::vector<Sensor> sensors;
    int x_min = 0, x_max = 0, y_min = 0, y_max = 0;
    parse_sensors(fn, sensors, x_min, x_max, y_min, y_max);
    //std::cout << x_min << ", " << x_max << ", " << y_min << ", " << y_max << '\n';

    Point current_pos{x_min, search_row};
    u_set beacons;
    for (auto const& s : sensors)
    {
        beacons.insert(s.position);
        beacons.insert(s.closest_beacon_position);
    }
    
    long cnt = 0;
    for (auto x = x_min - search_row; x <= x_max + search_row; x++)
    {
        current_pos.x = x;
        for (auto const& s : sensors)
            if (in_sensor_search_range(s, current_pos))
            {
                if (beacons.find(current_pos) == beacons.end())
                    cnt++;
                break;
            }
    }
   
    return cnt;
}

// it still takes more than 1 mins... need better algo
long long part_2(char const* fn, int search_row)
{
    std::vector<Sensor> sensors;
    int x_min = 0, x_max = 0, y_min = 0, y_max = 0;
    parse_sensors(fn, sensors, x_min, x_max, y_min, y_max);

    u_set beacons_nearby_points;
    for (auto const& s : sensors)
    {
        // try 1 outer
        int r = s.search_distance + 1;
        for (int i = 0; i < r; i++)
        {
            // 4 directions
            beacons_nearby_points.insert({s.position.x + i, s.position.y + (r - i)});
            beacons_nearby_points.insert({s.position.x - i, s.position.y - (r - i)});
            beacons_nearby_points.insert({s.position.x + (r - i), s.position.y - i});
            beacons_nearby_points.insert({s.position.x - (r - i), s.position.y + i});
        }
    }
    for (auto const& b : beacons_nearby_points)
    {
        if (std::none_of(sensors.begin(), sensors.end(), [&b](Sensor const& s){ return in_sensor_search_range(s, b); }) &&
            b.x >= 0 && b.x <= search_row && b.y >= 0 && b.y <= search_row)
        {
            //std::cout << b << '\n';
            return b.x * 4000000ll + b.y;
        }
    }
    return 0;
}


int main(int argc, char* argv[])
{
    
    // ./day_15.exe ./day15/input.txt 2000000
    // auto p = part_1(argv[1], std::atoi(argv[2]));
    // std::cout << "In the row where y=2000000, how many positions cannot contain a beacon?" << '\n'
    //           << p << std::endl;

    // ./day_15.exe ./day15/input.txt 4000000
    auto s = part_2(argv[1], std::atoi(argv[2]));
    std::cout << "What is its tuning frequency?" << '\n'
              << s << std::endl;

    return 0;
}