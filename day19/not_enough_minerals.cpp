//
// Created by Harold on 2022/12/19.
//

#include <fstream>
#include <string>
#include <cstdio>
#include <vector>
#include <deque>
#include <algorithm>
#include <iostream>

struct Robot
{
    int ore_cost = 0, clay_cost = 0, obsidian_cost = 0;
};

struct State
{
    int ore = 0, clay = 0, obsidian = 0, geode = 0;
    int ore_robot = 1, clay_robot = 0, obsidian_robot = 0, geode_robot = 0;

    int time = 24;

    void collect_resource()
    {
        ore += ore_robot;
        clay += clay_robot;
        obsidian += obsidian_robot;
        geode += geode_robot;

        time -= 1;
    }

    bool can_build(Robot const& robot)
    {
        return ore >= robot.ore_cost && clay >= robot.clay_cost && obsidian >= robot.obsidian_cost;
    }

    void build(Robot const& robot)
    {
        ore -= robot.ore_cost;
        clay -= robot.clay_cost;
        obsidian -= robot.obsidian_cost;
    }
};

struct BluePrint
{
    int id;
    Robot ore, clay, obsidian, geode;
    
    int get_quality_level() const
    {

        return id * get_max_geode_num();
    }
    int get_max_geode_num(State state = {}) const
    {
        auto enough_ore_robot_num = std::max({ore.ore_cost, clay.ore_cost, obsidian.ore_cost, geode.ore_cost});
        auto enough_clay_robot_num = std::max({ore.clay_cost, clay.clay_cost, obsidian.clay_cost, geode.clay_cost});
        std::deque<State> state_q{state};
        int max_geode_num = 0;
        while (!state_q.empty())
        {
            auto current_state = state_q.front();
            state_q.pop_front();
            if (current_state.time == 0)
            {
                max_geode_num = max_geode_num < current_state.geode ? current_state.geode : max_geode_num;
                continue;
            }
            if (current_state.can_build(ore) && current_state.ore_robot < enough_ore_robot_num)
            {
                auto new_state = current_state;
                new_state.build(ore);
                new_state.collect_resource();
                new_state.ore_robot++;
                state_q.push_back(new_state);
            }
            if (current_state.can_build(clay) && current_state.clay_robot < enough_clay_robot_num)
            {
                auto new_state = current_state;
                new_state.build(clay);
                new_state.collect_resource();
                new_state.clay_robot++;
                state_q.push_back(new_state);
            }
            if (current_state.can_build(geode))
            {
                auto new_state = current_state;
                new_state.build(geode);
                new_state.collect_resource();
                new_state.geode_robot++;
                state_q.push_back(new_state);
            }
            else if (current_state.can_build(obsidian))
            {
                auto new_state = current_state;
                new_state.build(obsidian);
                new_state.collect_resource();
                new_state.obsidian_robot++;
                state_q.push_back(new_state);
            }
            else if (current_state.ore <= enough_ore_robot_num)
            {
                auto new_state = current_state;
                new_state.collect_resource();
                state_q.push_back(new_state);
            }
        }
        return max_geode_num;
    }
};

std::ostream& operator<<(std::ostream& os, State const& state)
{
    os << "{\n[ore: " << state.ore << ", clay: " << state.clay << ", obsidian: " << state.obsidian << ", geode: " << state.geode << "]\n";
    os << "[ore_robot: " << state.ore_robot << ", clay_robot: " << state.clay_robot << ", obsidian_robot: " << state.obsidian_robot << ", geode_robot: " << state.geode_robot << "]\n";
    os << "time: " << state.time << "\n}";
    return os;
}

std::vector<BluePrint> parse_input(char const* fn)
{
    std::vector<BluePrint> blue_prints;
    std::ifstream ifs(fn);
    std::string line;
    int id = 0, a = 0, b = 0, c = 0, d = 0, e = 0, f = 0;
    while (std::getline(ifs, line))
    {
        std::sscanf(line.c_str(), "Blueprint %d: Each ore robot costs %d ore. Each clay robot costs %d ore. Each obsidian robot costs %d ore and %d clay. Each geode robot costs %d ore and %d obsidian.", &id, &a, &b, &c, &d, &e, &f);
        blue_prints.push_back({id, {a}, {b}, {c, d}, {e, 0, f}});
    }
    return blue_prints;
}

int part_1(char const* fn)
{
    auto blue_prints = parse_input(fn);
    int sum = 0;
    for (auto const& bp : blue_prints)
        sum += bp.get_quality_level();

    return sum;
}

long part_2(char const* fn)
{
    auto blue_prints = parse_input(fn);
    long mul = 1;
    State state{};
    state.time = 32;
    for (auto i = 0; i < 3; i++)
        mul *= blue_prints[i].get_max_geode_num(state);

    return mul;
}


int main(int argc, char* argv[])
{
    
    auto p = part_1(argv[1]);
    std::cout << "What do you get if you add up the quality level of all of the blueprints in your list?" << '\n'
              << p << std::endl;

    auto s = part_2(argv[1]);
    std::cout << "What do you get if you multiply these numbers together?" << '\n'
              << s << std::endl;

    return 0;
}