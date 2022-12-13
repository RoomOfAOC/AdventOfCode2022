//
// Created by Harold on 2022/12/13.
//

#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>

struct IntOrList
{
    int i = -1;
    std::vector<IntOrList> l;
};

int cmp(std::vector<IntOrList> const& l1, std::vector<IntOrList> const& l2);

int cmp(IntOrList const& i1, IntOrList const& i2)
{
    if (i1.i != -1 && i2.i != -1)
        return i1.i - i2.i;
    if (i1.i != -1)
        return cmp(std::vector<IntOrList>{ {i1.i} }, i2.l);
    if (i2.i != -1)
        return cmp(i1.l, std::vector<IntOrList>{ {i2.i} });
    return cmp(i1.l, i2.l);
}

int cmp(std::vector<IntOrList> const& l1, std::vector<IntOrList> const& l2)
{
	auto const n1 = static_cast<int>(l1.size());
    auto const n2 = static_cast<int>(l2.size());
    for (auto i = 0; i < n1 && i < n2; i++)
        if (auto d = cmp(l1[i], l2[i]); d != 0)
            return d;
    return n1 - n2;
}

int parse(std::string const& s, int idx, IntOrList& l)
{
    while (s[idx] != ']')
        if (s[idx] == '[')
        {
            IntOrList tmp;
            idx = parse(s, idx + 1, tmp);
            l.l.push_back(tmp);
        }
        else
        {
            if (s[idx] == ',')
            {
                idx++;
                continue;
            }
            std::string tmp{};
            while (std::isalnum(s[idx]))
                tmp += s[idx++];
            if (!tmp.empty())
                l.l.push_back(IntOrList{ std::stoi(tmp) });
        }
    return idx + 1;
}

bool is_right_order(std::string const& s1, std::string const& s2)
{
    IntOrList il1, il2;
    parse(s1, 1, il1);
    parse(s2, 1, il2);
    return cmp(il1, il2) < 0;
}

int part_1(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(ifs, line))
        if (!line.empty())
            lines.push_back(line);

    int sum = 0;
    for (auto i = 0, j = 1; i < lines.size() && j < lines.size(); i += 2, j += 2)
    	if (is_right_order(lines[i], lines[j]))
    		sum += i / 2 + 1;

    return sum;
}

int part_2(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    IntOrList il;
    while (std::getline(ifs, line))
        if (!line.empty())
        {
            IntOrList tmp;
            parse(line, 1, tmp);
            il.l.push_back(tmp);
        }
    auto two = IntOrList{ -1, std::vector<IntOrList>{ IntOrList{ -1, std::vector<IntOrList>{ IntOrList{2} } } }};
    auto six = IntOrList{ -1, std::vector<IntOrList>{ IntOrList{ -1, std::vector<IntOrList>{ IntOrList{6} } } }};
    il.l.push_back(two);
    il.l.push_back(six);
    
    std::sort(il.l.begin(), il.l.end(), [](IntOrList const& il1, IntOrList const& il2) {
        return cmp(il1, il2) < 0;
    });

    int idx2 = 0, idx6 = 0;
    for (auto i = 0; i < il.l.size(); i++)
        if (cmp(il.l[i], two) == 0)
            idx2 = i + 1;
        else if (cmp(il.l[i], six) == 0)
            idx6 = i + 1;

    //std::cout << idx2 << ", " << idx6 << '\n';

    return idx2 * idx6;
}


int main(int argc, char* argv[])
{
    
    auto p = part_1(argv[1]);
    std::cout << "What is the sum of the indices of those pairs?" << '\n'
              << p << std::endl;

    auto s = part_2(argv[1]);
    std::cout << "What is the decoder key for the distress signal?" << '\n'
              << s << std::endl;

    return 0;
}