//
// Created by Harold on 2022/12/20.
//

#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <iostream>

std::vector<int> parse_input(char const* fn)
{
    std::vector<int> numbers;
    std::ifstream ifs(fn);
    std::string line;
    while (std::getline(ifs, line))
        numbers.push_back(std::stoi(line));
    return numbers;
}


int part_1(char const* fn)
{
    auto numbers = parse_input(fn);
    auto const N = numbers.size();
    std::vector<std::pair<int, bool>> moved_numbers;
    moved_numbers.resize(N);
    std::transform(numbers.cbegin(), numbers.cend(), moved_numbers.begin(), [](int x){ return std::make_pair(x, false); });
    for (size_t idx = 0; idx < N;)
    {
        if (!moved_numbers[idx].second)
        {
            auto n = moved_numbers[idx].first;
            moved_numbers.erase(moved_numbers.begin() + idx);
            auto new_idx = (idx + n + 2 * (N - 1)) % (N - 1);
            //std::cout << idx << ", " << n << ", " << new_idx << '\n';
            moved_numbers.insert(moved_numbers.begin() + new_idx, std::make_pair(n, true));
        }
        else
            idx++;
    }

    auto pos_0 = std::find_if(moved_numbers.cbegin(), moved_numbers.cend(), [](auto const& p){ return p.first == 0; }) - moved_numbers.begin();

    //std::cout << moved_numbers[(pos_0 + 1000) % N].first << ", " << moved_numbers[(pos_0 + 2000) % N].first << ", " << moved_numbers[(pos_0 + 3000) % N].first << '\n';

    return moved_numbers[(pos_0 + 1000) % N].first + moved_numbers[(pos_0 + 2000) % N].first + moved_numbers[(pos_0 + 3000) % N].first;
}

long long part_2(char const* fn)
{
    constexpr long long key = 811589153;
    auto numbers = parse_input(fn);
    long long const N = numbers.size();
    std::list<std::pair<long long, size_t>> moved_numbers;
    moved_numbers.resize(N);
    size_t cnt = 0;
    std::transform(numbers.cbegin(), numbers.cend(), moved_numbers.begin(), [&cnt, key](int x){ return std::make_pair(x * key, cnt++); });
    for (int i = 0; i < 10; i++)
        for (size_t j = 0; j < N; j++)
        {
            auto it = std::find_if(moved_numbers.cbegin(), moved_numbers.cend(), [&j](auto const& p){ return p.second == j; });
            auto n = it->first;
            long long idx = std::distance(moved_numbers.cbegin(), it);
            moved_numbers.erase(it);
            auto new_idx = ((idx + n) % (N - 1) + (N - 1)) % (N - 1);
            //std::cout << idx << ", " << n << ", " << new_idx << '\n';
            moved_numbers.insert(std::next(moved_numbers.begin(), new_idx), std::make_pair(n, j));
        }

    auto pos_0 = std::distance(moved_numbers.cbegin(), std::find_if(moved_numbers.cbegin(), moved_numbers.cend(), [](auto const& p){ return p.first == 0; }));

    //std::cout << std::next(moved_numbers.begin(), (pos_0 + 1000) % N)->first << ", " << std::next(moved_numbers.begin(), (pos_0 + 2000) % N)->first << ", " << std::next(moved_numbers.begin(), (pos_0 + 3000) % N)->first << '\n';

    return std::next(moved_numbers.begin(), (pos_0 + 1000) % N)->first + std::next(moved_numbers.begin(), (pos_0 + 2000) % N)->first + std::next(moved_numbers.begin(), (pos_0 + 3000) % N)->first;
}


int main(int argc, char* argv[])
{
    
    auto p = part_1(argv[1]);
    std::cout << "What is the sum of the three numbers that form the grove coordinates?" << '\n'
              << p << std::endl;

    auto s = part_2(argv[1]);
    std::cout << "What is the sum of the three numbers that form the grove coordinates?" << '\n'
              << s << std::endl;

    return 0;
}