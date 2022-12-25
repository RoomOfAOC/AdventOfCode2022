//
// Created by Harold on 2022/12/25.
//

#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>

static std::unordered_map<char, long long> FromDigits
{
    {'2', 2},
    {'1', 1},
    {'0', 0},
    {'-', -1},
    {'=', -2}
};

static std::unordered_map<long long, char> ToDigits
{
    {2, '2'},
    {1, '1'},
    {0, '0'},
    {-1, '-'},
    {-2, '='}
};

long long SNAFU_to_decimal(std::string const& s)
{
    long long mul = 1;
    long long n = 0;
    for (auto it = s.crbegin(); it != s.crend(); it++)
    {
        n += FromDigits[*it] * mul;
        mul *= 5;
    }
    return n;
}

std::string decimal_to_SNAFU(long long n)
{   
    switch (n % 5)
    {
    case 0:
        return n == 0 ? "" : decimal_to_SNAFU(n / 5) + ToDigits[0];
    case 1:
        return decimal_to_SNAFU(n / 5) + ToDigits[1];
    case 2:
        return decimal_to_SNAFU(n / 5) + ToDigits[2];
    case 3:
        return decimal_to_SNAFU((n + 2) / 5) + ToDigits[-2];
    case 4:
        return decimal_to_SNAFU((n + 1) / 5) + ToDigits[-1];
    
    default:
        throw "wrong";
    }
}

std::vector<std::string> parse_input(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(ifs, line))
        lines.push_back(line);
    return lines;
}

std::string part_1(char const* fn)
{
    long long sum = 0;
    auto numbers = parse_input(fn);
    for (auto const& n : numbers)
        sum += SNAFU_to_decimal(n);
    
    return decimal_to_SNAFU(sum);
}


int main(int argc, char* argv[])
{
    
    auto p = part_1(argv[1]);
    std::cout << "What SNAFU number do you supply to Bob's console?" << '\n'
              << p << std::endl;

    return 0;
}