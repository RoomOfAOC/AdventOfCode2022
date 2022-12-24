//
// Created by Harold on 2022/12/24.
//

#include <fstream>
#include <string>
#include <array>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <iostream>

#include "../day12/mat.h"

using PointSet = std::unordered_set<Point, Point::PointHasher>;

struct Move
{
    enum class Direction
    {
        L, R, U, D
    };

    Direction dir;

    Point move(Point const& pos)
    {
        Point new_pos = pos;
        switch (dir)
        {
        case Direction::L:
            new_pos.x -= 1;
            break;
        case Direction::R:
            new_pos.x += 1;
            break;
        case Direction::U:
            new_pos.y -= 1;
            break;
        case Direction::D:
            new_pos.y += 1;
            break;
        
        default:
            break;
        }
        return new_pos;
    }
};

struct Blizzard
{
    Point pos;
    Move mov;
};

std::ostream& operator<<(std::ostream& os, Move const& mov)
{
    switch (mov.dir)
    {
    case Move::Direction::L:
        os << "L";
        break;
    case Move::Direction::R:
        os << "R";
        break;
    case Move::Direction::U:
        os << "U";
        break;
    case Move::Direction::D:
        os << "D";
        break;
    
    default:
        break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, Blizzard const& b)
{
    return os << "{" << b.pos << ", " << b.mov << "}";
}

struct Valley
{
    Point entrance, exit;
    std::vector<Blizzard> blizzards;
    PointSet walls;
    int max_x, max_y;

    bool valid_pos(Point const& p)
    {
        return p.x >= 0 && p.x < max_x && p.y >= 0 && p.y < max_y;
    }
};

std::ostream& operator<<(std::ostream& os, Valley const& v)
{
    os << "{\n\t" << v.entrance << ", " << v.exit << "\n\t";
    for (auto const& b : v.blizzards)
        os << b << " ";
    os << '\n';
    for (auto const& w : v.walls)
        os << w << " ";
    os << "\n\t" << v.max_x << ", " << v.max_y << "\n}";
    return os;
}

Valley parse_input(char const* fn)
{
    Valley valley;
    std::ifstream ifs(fn);
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(ifs, line))
        lines.push_back(line);
    auto N = static_cast<int>(lines.size());
    auto M = static_cast<int>(line.size());
    valley.max_x = M;
    valley.max_y = N;
    valley.blizzards.reserve(N - 2);
    for (auto y = 0; y < N; y++)
    {
        if (y == 0)
        {
            for (auto x = 0; x < M; x++)
                if (lines[y][x] == '.')
                    valley.entrance = {x, y};
                else
                    valley.walls.insert({x, y});
            continue;
        }
        else if (y == N - 1)
        {
            for (auto x = 0; x < M; x++)
                if (lines[y][x] == '.')
                    valley.exit = {x, y};
                else
                    valley.walls.insert({x, y});
            continue;
        }
        else
            for (auto x = 0; x < line.size(); x++)
            {
                if (auto c = lines[y][x]; c == '#')
                    valley.walls.insert({x, y});
                else if (c != '.')
                {
                    Move m;
                    switch (c)
                    {
                    case '<':
                        m.dir = Move::Direction::L;
                        break;
                    case '>':
                        m.dir = Move::Direction::R;
                        break;
                    case '^':
                        m.dir = Move::Direction::U;
                        break;
                    case 'v':
                        m.dir = Move::Direction::D;
                        break;
                    
                    default:
                        break;
                    }
                    valley.blizzards.push_back({{x, y}, m});
                }
            }
    }

    return valley;
}

void move_one_minute(Valley& valley)
{
    for (auto& b : valley.blizzards)
    {
        Point new_pos = b.mov.move(b.pos);
        if (valley.walls.find(new_pos) == valley.walls.end())
            b.pos = new_pos;
        else
        {
            switch (b.mov.dir)
            {
            case Move::Direction::L:
                b.pos.x = valley.max_x - 2;
                break;
            case Move::Direction::R:
                b.pos.x = 1;
                break;
            case Move::Direction::U:
                b.pos.y = valley.max_y - 2;
                break;
            case Move::Direction::D:
                b.pos.y = 1;
                break;
            
            default:
                break;
            }
        }
    }
}

int part_1(char const* fn)
{
    auto valley = parse_input(fn);
    //std::cout << valley << '\n';
    PointSet current_valid_pos;
    current_valid_pos.insert(valley.entrance);
    int time = 0;
    std::array<Move, 4> moves{Move{Move::Direction::L}, {Move::Direction::R}, {Move::Direction::U}, {Move::Direction::D}};
    while (true)
    {
        time++;
        move_one_minute(valley);
        PointSet blizzard_pos, next_valid_pos;
        for (auto const& b : valley.blizzards)
            blizzard_pos.insert(b.pos);
        for (auto const& pos : current_valid_pos)
        {
            if (blizzard_pos.find(pos) == blizzard_pos.end())
                next_valid_pos.insert(pos);
            for (auto m : moves)
            {
                auto new_pos = m.move(pos);
                if (valley.valid_pos(new_pos) &&
                    valley.walls.find(new_pos) == valley.walls.end() &&
                    blizzard_pos.find(new_pos) == blizzard_pos.end())
                {
                    next_valid_pos.insert(new_pos);
                    if (new_pos == valley.exit)
                        return time;
                }
            }
        }
        current_valid_pos = next_valid_pos;
    }
    
    return time;
}

int part_2(char const* fn)
{
    auto valley = parse_input(fn);
    PointSet current_valid_pos;
    current_valid_pos.insert(valley.entrance);
    int time = 0;
    std::array<Move, 4> moves{Move{Move::Direction::L}, {Move::Direction::R}, {Move::Direction::U}, {Move::Direction::D}};
    int cnt = 0;
    while (true)
    {
        time++;
        move_one_minute(valley);
        PointSet blizzard_pos, next_valid_pos;
        for (auto const& b : valley.blizzards)
            blizzard_pos.insert(b.pos);
        for (auto const& pos : current_valid_pos)
        {
            if (blizzard_pos.find(pos) == blizzard_pos.end())
                next_valid_pos.insert(pos);
            for (auto m : moves)
            {
                auto new_pos = m.move(pos);
                if (valley.valid_pos(new_pos) &&
                    valley.walls.find(new_pos) == valley.walls.end() &&
                    blizzard_pos.find(new_pos) == blizzard_pos.end())
                {
                    next_valid_pos.insert(new_pos);
                    if ((new_pos == valley.exit && cnt == 0) ||
                        (new_pos == valley.entrance && cnt == 1))
                        {
                            next_valid_pos.clear();
                            next_valid_pos.insert(new_pos);
                            cnt++;
                            goto NEXT;
                        }
                    else if (new_pos == valley.exit && cnt == 2)
                        return time;
                }
            }
        }
NEXT:
        current_valid_pos = next_valid_pos;
    }
    
    return time;
}


int main(int argc, char* argv[])
{
    
    auto p = part_1(argv[1]);
    std::cout << "What is the fewest number of minutes required to avoid the blizzards and reach the goal?" << '\n'
              << p << std::endl;

    auto s = part_2(argv[1]);
    std::cout << "What is the fewest number of minutes required to reach the goal, go back to the start, then reach the goal again?" << '\n'
              << s << std::endl;

    return 0;
}