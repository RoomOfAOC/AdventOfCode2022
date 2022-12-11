//
// Created by Harold on 2022/12/11.
//

#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <functional>
#include <algorithm>
#include <iostream>

using ull = unsigned long long;

struct Operation
{
    enum class OP
    {
        Add,
        Mul
    };
    OP op;
    ull operand;
};

struct Test
{
    unsigned divisor;
    unsigned true_pass, false_pass;
};

struct Monkey
{
    std::deque<ull> items;
    Operation op;
    Test test;
    ull inspection_times = 0;
    std::function<void()> inspect = [](){};
};

std::vector<Monkey> ParseMonkeys(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    Monkey monkey{};
    unsigned cnt = 0;
    while (std::getline(ifs, line))
        if (line[0] == 'M')
            cnt++;
    std::vector<Monkey> monkeys(cnt);
    cnt = 0;
    ifs = std::ifstream(fn);
    while (std::getline(ifs, line))
    {
        if (line.empty())
            continue;
        if (line[0] == 'M')
            monkey.items.clear();
        else if (line[2] == 'S')
        {
            auto nums = line.substr(18);
            std::string tmp{};
            for (auto c : nums)
                if (std::isalnum(c))
                    tmp += c;
                else if (!tmp.empty())
                {
                    monkey.items.push_back(std::stoull(tmp));
                    tmp.clear();
                }
            monkey.items.push_back(std::stoull(tmp));
        }
        else if (line[2] == 'O')
        {
            auto op_pos = line.find_first_of("d") + 2;
            monkey.op.op = line[op_pos] == '+' ? Operation::OP::Add : Operation::OP::Mul;
            monkey.op.operand = line[op_pos + 2] == 'o' ? 0 : std::stoull(line.substr(op_pos + 2));
        }
        else if (line[2] == 'T')
            monkey.test.divisor = std::stoul(line.substr(line.find('y') + 2));
        else if (line[7] == 't')
            monkey.test.true_pass = std::stoul(line.substr(line.find('y') + 2));
        else if (line[7] == 'f')
        {
            monkey.test.false_pass = std::stoul(line.substr(line.find('y') + 2));
            monkeys[cnt++] = monkey;
        }
    }
    return monkeys;
}

ull CountMonkeyBusiness(std::vector<Monkey> const& monkeys, unsigned rounds)
{
    for (unsigned i = 0; i < rounds; i++)
    {
        for (auto& m : monkeys)
            m.inspect();
    }
    std::vector<ull> inspection_times(monkeys.size());
    std::transform(monkeys.cbegin(), monkeys.cend(), inspection_times.begin(), [](Monkey const& m){ return m.inspection_times; });
    std::partial_sort(inspection_times.begin(), inspection_times.begin() + 2, inspection_times.end(), std::greater<ull>{});

    return inspection_times[0] * inspection_times[1];
}

ull part_1(char const* fn)
{
    auto monkeys = ParseMonkeys(fn);
    for (size_t i = 0; i < monkeys.size(); i++)
    {
        monkeys[i].inspect = [&monkeys, i]()
        {
            Monkey &m = monkeys[i];
            while (!m.items.empty())
            {
                auto item = m.items.front();
                if (m.op.op == Operation::OP::Add)
                    item += m.op.operand == 0 ? item : m.op.operand;
                else
                    item *= m.op.operand == 0 ? item : m.op.operand;
                item /= 3;
                m.items.pop_front();
                if (item % m.test.divisor == 0)
                    monkeys[m.test.true_pass].items.push_back(item);
                else
                    monkeys[m.test.false_pass].items.push_back(item);
                m.inspection_times++;
            }
        };
    }

    return CountMonkeyBusiness(monkeys, 20);
}

ull part_2(char const* fn)
{
    auto monkeys = ParseMonkeys(fn);
    ull divisor = 1;
    for (auto const& m : monkeys)
        divisor *= m.test.divisor;
    for (size_t i = 0; i < monkeys.size(); i++)
    {
        monkeys[i].inspect = [&monkeys, divisor, i]()
        {
            Monkey &m = monkeys[i];
            while (!m.items.empty())
            {
                auto item = m.items.front();
                if (m.op.op == Operation::OP::Add)
                    item += m.op.operand == 0 ? item : m.op.operand;
                else
                    item *= m.op.operand == 0 ? item : m.op.operand;
                item %= divisor;
                m.items.pop_front();
                if (item % m.test.divisor == 0)
                    monkeys[m.test.true_pass].items.push_back(item);
                else
                    monkeys[m.test.false_pass].items.push_back(item);
                m.inspection_times++;
            }
        };
    }

    return CountMonkeyBusiness(monkeys, 10000);
}

int main(int argc, char* argv[])
{
    
    auto p = part_1(argv[1]);
    std::cout << "What is the level of monkey business after 20 rounds of stuff-slinging simian shenanigans?" << '\n'
              << p << std::endl;

    auto s = part_2(argv[1]);
    std::cout << "what is the level of monkey business after 10000 rounds?" << '\n'
              << s << std::endl;

    return 0;
}