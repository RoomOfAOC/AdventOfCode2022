//
// Created by Harold on 2022/12/9.
//

#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <iostream>

struct Point
{
    int x = 0;
    int y = 0;

    Point() = default;
    Point(int x, int y) : x(x), y(y) { }
    Point(Point const&) = default;
    Point& operator=(Point const&) = default;
    Point(Point&&) = default;
    Point& operator=(Point&&) = default;

    bool operator==(Point const& other) const
    {
        return x == other.x && y == other.y;
    }

    struct Hasher
    {
        std::size_t operator()(Point const& p) const noexcept
        {
            return std::hash<int>{}(p.x) ^ (std::hash<int>{}(p.y) << 1);
        }
    };
};

std::ostream& operator<<(std::ostream& os, Point const& p)
{
    os << "[" << p.x << ", " << p.y << "]";
    return os;
}

int get_d2(Point const& p1, Point const& p2)
{
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

size_t part_1(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    std::unordered_set<Point, Point::Hasher> set;
    Point H, T;
    set.insert(T);
    int moves = 0;
    while (std::getline(ifs, line))
    {
        moves = std::stoi(line.substr(2));
        for (int i = 0; i < moves; i++)
        {
            switch (line[0])
            {
            case 'L':
                H.x--;
                break;
            case 'R':
                H.x++;
                break;
            case 'U':
                H.y++;
                break;
            case 'D':
                H.y--;
                break;
            
            default:
                break;
            }

            if (get_d2(H, T) > 2)
            {
                Point t;
                int dmin = 3;
                // 9 possible pos
                for (int dx = -1; dx <= 1; dx++)
                    for (int dy = -1; dy <= 1; dy++)
                    {
                        Point tmp(T.x + dx, T.y + dy);
                        int d = get_d2(H, tmp);
                        if (d < dmin)
                        {
                            t = tmp;
                            dmin = d;
                        }
                    }
                T = t;
                set.insert(t);
            }
        }
    }
    // for (auto const& p : set)
    //     std::cout << p << '\n';
    return set.size();
}

size_t part_2(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    std::unordered_set<Point, Point::Hasher> set;
    Point H, T;
    set.insert(T);
    int moves = 0;
    std::vector<Point> tails(9);
    while (std::getline(ifs, line))
    {
        moves = std::stoi(line.substr(2));
        for (int i = 0; i < moves; i++)
        {
            switch (line[0])
            {
            case 'L':
                H.x--;
                break;
            case 'R':
                H.x++;
                break;
            case 'U':
                H.y++;
                break;
            case 'D':
                H.y--;
                break;
            
            default:
                break;
            }

            Point head = H;
            // 9 tail nodes
            for (int j = 0; j < 9; j++)
            {
                T = tails[j];
                if (get_d2(head, T) > 2)
                {
                    Point t;
                    int dmin = 3;
                    // 9 possible pos
                    for (int dx = -1; dx <= 1; dx++)
                        for (int dy = -1; dy <= 1; dy++)
                        {
                            Point tmp(T.x + dx, T.y + dy);
                            int d = get_d2(head, tmp);
                            if (d < dmin)
                            {
                                t = tmp;
                                dmin = d;
                            }
                        }
                    tails[j] = t;
                }
                head = tails[j];
            }
            set.insert(tails[8]);
        }
    }
    // for (auto const& p : set)
    //     std::cout << p << '\n';
    return set.size();
}

int main(int argc, char* argv[])
{
    
    auto p = part_1(argv[1]);
    std::cout << "How many positions does the tail of the rope visit at least once?" << '\n'
              << p << std::endl;

    p = part_2(argv[1]);
    std::cout << "How many positions does the tail of the rope visit at least once?" << '\n'
              << p << std::endl;

    return 0;
}