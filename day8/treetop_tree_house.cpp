//
// Created by Harold on 2022/12/8.
//

#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <iostream>

size_t part_1(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    std::vector<std::string> grid_rows;
    std::vector<std::string> grid_columns;
    size_t sum = 0;
    while (std::getline(ifs, line))
        grid_rows.push_back(line);
    auto height = grid_rows.size();
    auto width = grid_rows[0].size();
    sum += 2 * (width + height) - 4;
    grid_columns.resize(width, std::string(height, '0'));
    for (auto r = 0; r < height; r++)
        for (auto c = 0; c < width; c++)
            grid_columns[c][r] = grid_rows[r][c];
    for (auto r = 1; r < height - 1; r++)
    {
        auto const& row = grid_rows[r];
        for (auto c = 1; c < width - 1; c++)
        {
            auto const& col = grid_columns[c];
            if ((std::max_element(row.begin(), row.begin() + c + 1) == row.begin() + c) ||
                (std::max_element(row.rbegin(), row.rbegin() + width - c) == row.rbegin() + width - c - 1) || 
                (std::max_element(col.begin(), col.begin() + r + 1) == col.begin() + r) ||
                (std::max_element(col.rbegin(), col.rbegin() + height - r) == col.rbegin() + height - r - 1))
            {
                //std::cout << "[" << r << ", " << c << "]\n";
                sum += 1;
            }
        }
    }
    
    return sum;
}

size_t part_2(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    std::vector<std::string> grid_rows;
    std::vector<std::string> grid_columns;
    size_t score = 0;
    while (std::getline(ifs, line))
        grid_rows.push_back(line);
    auto height = grid_rows.size();
    auto width = grid_rows[0].size();
    grid_columns.resize(width, std::string(height, '0'));
    for (auto r = 0; r < height; r++)
        for (auto c = 0; c < width; c++)
            grid_columns[c][r] = grid_rows[r][c];
    for (auto r = 1; r < height - 1; r++)
    {
        auto const& row = grid_rows[r];
        for (auto c = 1; c < width - 1; c++)
        {
            auto const& col = grid_columns[c];
            auto n = row[c];
            size_t left = 0, right = 0, up = 0, down = 0;
            for (int it = c - 1; it >= 0; it--)
            {
                left++;
                if (row[it] >= n)
                    break;
            }
            for (int it = c + 1; it < width; it++)
            {
                right++;
                if (row[it] >= n)
                    break;
            }
            for (int it = r - 1; it >= 0; it--)
            {
                up++;
                if (col[it] >= n)
                    break;
            }
            for (int it = r + 1; it < height; it++)
            {
                down++;
                if (col[it] >= n)
                    break;
            }
            auto s = left * right * up * down;
            //std::cout << "[" << r << ", " << c << "]: " << left << ", " << right << ", " << up << ", " << down << " - " << s << '\n';
            if (s > score)
                score = s;
        }
    }
    
    return score;
}

int main(int argc, char* argv[])
{
    
    auto p = part_1(argv[1]);
    std::cout << "how many trees are visible from outside the grid?" << '\n'
              << p << std::endl;

    p = part_2(argv[1]);
    std::cout << "What is the highest scenic score possible for any tree?" << '\n'
              << p << std::endl;

    return 0;
}