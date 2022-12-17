//
// Created by Harold on 2022/12/17.
//

#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <array>
#include <unordered_map>

#include "../day12/mat.h"

std::string parse_input(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    std::getline(ifs, line);
    return line;
}

using OccupiedPointsSet = std::unordered_set<Point, Point::PointHasher>;

struct Rock
{
    std::vector<Point> points;

    bool check_collision(OccupiedPointsSet& ops, int x, int y)
    {
        for (auto const& p : points)
        {
            if (x + p.x < 1 || x + p.x > 7)
                return true;
            if (y + p.y < 1)
                return true;
            if (ops.find({x + p.x, y + p.y}) != ops.end())
                return true;
        }
        return false;
    }
};

// left down as (0, 0)
Rock rock_1{{{0, 0}, {1, 0}, {2, 0}, {3, 0}}};
Rock rock_2{{{1, 0}, {0, 1}, {1, 1}, {2, 1}, {1, 2}}};
Rock rock_3{{{2, 0}, {2, 1}, {0, 0}, {1, 0}, {2, 2}}};
Rock rock_4{{{0, 0}, {0, 1}, {0, 2}, {0, 3}}};
Rock rock_5{{{0, 0}, {1, 0}, {0, 1}, {1, 1}}};

std::vector<Rock> rocks = {rock_1, rock_2, rock_3, rock_4, rock_5};

int part_1(char const* fn, int rocks_num = 2022)
{
    auto gas = parse_input(fn);
    int x = 3, y = 0;
    OccupiedPointsSet ops{};
    int cnt = 0;
    for (int i = 0; i < rocks_num; i++)
    {
        x = 3;
        y += 4;
        Rock& rock = rocks[i % 5];
        char c;
        while (true)
        {
            // move right
            if (c = gas[cnt++ % gas.size()]; c == '>')
            {
                if (!rock.check_collision(ops, x + 1, y))
                    x++;
            }
            // move left
            else
            {
                if (!rock.check_collision(ops, x - 1, y))
                    x--;
            }
            // move down
            if (!rock.check_collision(ops, x, y - 1))
                y--;
            else 
                break;
        }
        for (auto const &p : rock.points)
            ops.insert({p.x + x, p.y + y});
        y = std::max_element(ops.begin(), ops.end(), [](Point const& p1, Point const& p2) { return p1.y < p2.y; })->y;
    }
    
    return y;
}

using Top = std::array<int, 7>;
struct TopHasher
{
    size_t operator()(Top const &top) const
    {
        std::hash<bool> hasher;
        size_t res = 0;
        for (int i = 0; i < 7; i++)
            res = (res << 1) ^ hasher(top[i]);
        return res;
    }
};

using TopSet = std::unordered_set<Top, TopHasher>;

struct ChamberState
{
    int rock_idx;
    int gas_idx;
    Top top;

    bool operator==(ChamberState const& other) const
    {
        return rock_idx == other.rock_idx && gas_idx == other.gas_idx && top == other.top;
    }

    struct Hasher
    {
        size_t operator()(ChamberState const& s) const
        {
            return std::hash<int>()(s.rock_idx) ^ std::hash<int>()(s.gas_idx) ^ TopHasher()(s.top);
        }
    };
};

std::ostream& operator<<(std::ostream& os, ChamberState const& s)
{
    os << "{\n\t" << s.rock_idx << ", " << s.gas_idx << "\n[";
    for (auto i = 0; i < 6; i++)
        os << s.top[i] << ' ';
    os << s.top[6] << "]\n}";
    return os;
}

struct RockState
{
    long long rock_num;
    long long height;
};

long long part_2(char const* fn, long long rocks_num = 1000000000000)
{
    auto gas = parse_input(fn);
    int x = 3, y = 0;
    OccupiedPointsSet ops{};
    int cnt = 0;
    std::unordered_map<ChamberState, RockState, ChamberState::Hasher> visited;
    long long repeated_heights = 0;
    for (long long i = 0; i < rocks_num; i++)
    {
        x = 3;
        y += 4;
        Rock& rock = rocks[i % 5];
        char c;
        while (true)
        {
            // move right
            if (c = gas[cnt++ % gas.size()]; c == '>')
            {
                if (!rock.check_collision(ops, x + 1, y))
                    x++;
            }
            // move left
            else
            {
                if (!rock.check_collision(ops, x - 1, y))
                    x--;
            }
            // move down
            if (!rock.check_collision(ops, x, y - 1))
                y--;
            else 
                break;
        }
        for (auto const &p : rock.points)
            ops.insert({p.x + x, p.y + y});
        
        y = std::max(y, std::max_element(ops.begin(), ops.end(), [](Point const& p1, Point const& p2) { return p1.y < p2.y; })->y);

        ChamberState state{
            i % 5,
            (cnt - 1) % gas.size(),
        };
        for (auto xx = 1; xx < 8; xx++)
        {
            int tmp = 0;
            for (auto [xxx, yyy] : ops)
                if (xx == xxx && yyy > tmp)
                    tmp = yyy;
            state.top[xx - 1] = y - tmp;
        }
        //std::cout << i << "\n" << state << '\n';
        if (visited.find(state) == visited.end())
            visited[state] = RockState{i, y};
        else
        {
            auto [rock_num, height] = visited[state];
            auto di = i - rock_num;
            auto dy = y - height;
            auto repeats = (rocks_num - i) / di;
            repeated_heights += repeats * dy;
            i += repeats * di;
            // std::cout << di << ", " << dy << ", " << repeated_heights << ", " << i << '\n';
            // break; 
        }
    }
    
    return y + repeated_heights;
}


int main(int argc, char* argv[])
{
    
    auto p = part_1(argv[1]);
    std::cout << "How many units tall will the tower of rocks be after 2022 rocks have stopped falling?" << '\n'
              << p << std::endl;

    auto s = part_2(argv[1]);
    std::cout << "How tall will the tower be after 1000000000000 rocks have stopped?" << '\n'
              << s << std::endl;

    return 0;
}