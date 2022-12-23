//
// Created by Harold on 2022/12/22.
//

#include <fstream>
#include <string>
#include <vector>
#include <variant>
#include <unordered_map>
#include <algorithm>
#include <iostream>

#include "../day12/mat.h"

using MonkeyMap = Mat<char>;

std::pair<MonkeyMap, std::string> parse_input(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    int rows = 0, cols = 0;
    while (std::getline(ifs, line))
    {
        if (line.empty()) break;
        rows++;
        cols = line.size() > cols ? line.size() : cols;
    }
    std::getline(ifs, line);
    auto path = line;
    MonkeyMap mm(rows + 2, cols + 2, '@');
    ifs.clear();
    ifs.seekg(0);
    rows = 1;
    while (std::getline(ifs, line))
    {
        if (line.empty()) break;
        cols = 1;
        for (auto c : line)
        {
            if (c != ' ')
                mm(rows, cols) = c;
            cols++;
        }
        rows++;
    }
    return std::make_pair(mm, path);
}

struct Position
{
    int row, column, facing;
};

std::ostream& operator<<(std::ostream& os, Position const& pos)
{
    return os << "[" << pos.row << ", " << pos.column << ", " << pos.facing << "]";
}

struct Move
{
    int facing, steps;
};

std::ostream& operator<<(std::ostream& os, Move const& move)
{
    return os << "[" << move.facing << ", " << move.steps << "]";
}

std::vector<Move> parse_moves(std::string const& path)
{
    std::vector<Move> moves;
    std::string tmp{};
    int facing = 0;
    int steps = 0;
    for (auto c : path)
        if (!std::isalpha(static_cast<unsigned char>(c)))
            tmp += c;
        else
        {
            if (!tmp.empty()) steps = std::stoi(tmp);
            moves.push_back({facing, steps});
            tmp.clear();
            if (c == 'R')
                facing++;
            else
                facing--;
            if (facing == -1) facing = 3;
            else if (facing == 4) facing = 0;
        }
    if (!tmp.empty()) moves.push_back({facing, std::stoi(tmp)});
    return moves;
}

Position move_one_step(Position const& pos)
{
    auto new_pos = pos;
    switch (new_pos.facing)
    {
    case 0:
        new_pos.column++;
        break;
    case 1:
        new_pos.row++;
        break;
    case 2:
        new_pos.column--;
        break;
    case 3:
        new_pos.row--;
        break;
    
    default:
        break;
    }
    return new_pos;
}

Position reverse_move_one_step(Position const& pos)
{
    auto new_pos = pos;
    switch (new_pos.facing)
    {
    case 0:
        new_pos.column--;
        break;
    case 1:
        new_pos.row--;
        break;
    case 2:
        new_pos.column++;
        break;
    case 3:
        new_pos.row++;
        break;
    
    default:
        break;
    }
    return new_pos;
}

void move(Position& pos, Move const& move, MonkeyMap const& mm)
{
    pos.facing = move.facing;
    for (auto i = 0; i < move.steps; i++)
    {
        auto new_pos = move_one_step(pos);
        if (mm(new_pos.row, new_pos.column) == '@')
        {
            while (true)
            {
                if (auto rev_pos = reverse_move_one_step(new_pos); mm(rev_pos.row, rev_pos.column) != '@')
                    new_pos = rev_pos;
                else
                    break;
            }
        }
        if (mm(new_pos.row, new_pos.column) == '#')
            break;
        pos = new_pos;
    }
}

int part_1(char const* fn)
{
    auto [mm, path] = parse_input(fn);
    auto moves = parse_moves(path);
    Position pos;
    for (int i = 1; i < mm.cols; i++)
        if (mm(1, i) == '.')
        {
            pos = { 1, i, 0 };
            break;
        }
    for (auto const& m : moves)
        move(pos, m, mm);

    return pos.row * 1000 + pos.column * 4 + pos.facing;
}


int main(int argc, char* argv[])
{
    
    auto p = part_1(argv[1]);
    std::cout << "What is the final password?" << '\n'
              << p << std::endl;

    return 0;
}