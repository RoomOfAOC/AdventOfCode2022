//
// Created by Harold on 2022/12/21.
//

#include <fstream>
#include <string>
#include <vector>
#include <variant>
#include <unordered_map>
#include <algorithm>
#include <iostream>

struct Operation
{
    std::string op1, op2;
    char op;
};

using Monkey = std::variant<long long, Operation>;
using MonkeyMap = std::unordered_map<std::string, Monkey>;

MonkeyMap parse_input(char const* fn)
{
    MonkeyMap mm;
    std::ifstream ifs(fn);
    std::string line;
    while (std::getline(ifs, line))
    {
        auto comma_pos = line.find(':');
        auto name = line.substr(0, comma_pos);
        auto op = line.substr(comma_pos + 2);
        if (op[0] >= '0' && op[0] <= '9')
            mm[name] = std::stoll(op);
        else
            mm[name] = Operation{ op.substr(0, 4), op.substr(7, 4), op[5] };
    }
    return mm;
}

long long part_1(char const* fn)
{
    auto mm = parse_input(fn);
    MonkeyMap mm1;
    while (mm1.find("root") == mm1.end())
    {
        for (auto const& [k, v] : mm)
        if (mm1.find(k) == mm1.end())
        {
            if (std::holds_alternative<long long>(v))
                mm1[k] = std::get<long long>(v);
            else
            {
                auto op = std::get<Operation>(v);
                if (mm1.find(op.op1) != mm1.end() && mm1.find(op.op2) != mm1.end())
                {
                    switch (op.op)
                    {
                    case '+':
                        mm1[k] = std::get<long long>(mm1[op.op1]) + std::get<long long>(mm1[op.op2]);
                        break;
                    case '-':
                        mm1[k] = std::get<long long>(mm1[op.op1]) - std::get<long long>(mm1[op.op2]);
                        break;
                    case '*':
                        mm1[k] = std::get<long long>(mm1[op.op1]) * std::get<long long>(mm1[op.op2]);
                        break;
                    case '/':
                        mm1[k] = std::get<long long>(mm1[op.op1]) / std::get<long long>(mm1[op.op2]);
                        break;
                    
                    default:
                        break;
                    }
                }
            }
        }
    }
        
    return std::get<long long>(mm1["root"]);
}

bool get_calculation_path(std::string from, std::string to, MonkeyMap& mm, std::vector<std::string>& calculation_path)
{
    if (from == to)
        return true;
    if (std::holds_alternative<long long>(mm[from]))
        return false;
    auto op = std::get<Operation>(mm[from]);
    calculation_path.push_back(op.op1);
    if (get_calculation_path(op.op1, to, mm, calculation_path))
        return true;
    calculation_path.pop_back();
    calculation_path.push_back(op.op2);
    if (get_calculation_path(op.op2, to, mm, calculation_path))
        return true;
    calculation_path.pop_back();
    return false;
}

long long evaluate(std::string name, MonkeyMap& mm)
{
    if (std::holds_alternative<long long>(mm[name]))
        return std::get<long long>(mm[name]);
    auto op = std::get<Operation>(mm[name]);
    auto left = evaluate(op.op1, mm);
    auto right = evaluate(op.op2, mm);
    switch (op.op)
    {
    case '+':
        return left + right;
    case '-':
        return left - right;
    case '*':
        return left * right;
    case '/':
        return left / right;
    
    default:
        throw "wrong";
    }
}

// reverse evaluate
long long evaluate(long long op1, long long op2, Operation const& op, bool is_at_equation_left)
{
    switch (op.op)
    {
    case '+':
        return op1 - op2;
    case '-':
        return is_at_equation_left ? op1 + op2 : op2 - op1;
    case '*':
        return op1 / op2;
    case '/':
        return is_at_equation_left ? op1 * op2 : op2 / op1;
    
    default:
        throw "wrong";
    }
}

long long evaluate(long long num, std::string name, size_t idx, std::vector<std::string>& path, MonkeyMap& mm)
{
    if (idx == path.size())
        return num;
    auto op = std::get<Operation>(mm[name]);
    auto current = path[idx];
    if (current == op.op1)
    {
        auto op1 = evaluate(op.op2, mm);
        auto op2 = evaluate(num, op1, op, true);
        return evaluate(op2, op.op1, idx + 1, path, mm);
    }
    else
    {
        auto op1 = evaluate(op.op1, mm);
        auto op2 = evaluate(num, op1, op, false);
        return evaluate(op2, op.op2, idx + 1, path, mm);
    }
}

long long part_2(char const* fn)
{
    auto mm = parse_input(fn);
    std::vector<std::string> path;
    get_calculation_path("root", "humn", mm, path);
    // for (auto const& n : path)
    //     std::cout << n << '\n';
    auto op = std::get<Operation>(mm["root"]);
    if (path[0] == op.op1)
    {
        auto op1 = evaluate(op.op2, mm);
        return evaluate(op1, op.op1, 1, path, mm);
    }
    else
    {
        auto op1 = evaluate(op.op1, mm);
        return evaluate(op1, op.op2, 1, path, mm);
    }
}


int main(int argc, char* argv[])
{
    
    auto p = part_1(argv[1]);
    std::cout << "What number will the monkey named root yell?" << '\n'
              << p << std::endl;

    auto s = part_2(argv[1]);
    std::cout << "What number do you yell to pass root's equality test?" << '\n'
              << s << std::endl;

    return 0;
}