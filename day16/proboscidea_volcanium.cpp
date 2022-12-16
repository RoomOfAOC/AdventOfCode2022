//
// Created by Harold on 2022/12/16.
//

#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <algorithm>
#include <numeric>
#include <iostream>

struct Valve
{
    std::string name;
    int flow_rate;
    std::vector<std::string> valves_tunnels_to;
};

using ValveMap = std::unordered_map<std::string, Valve>;

std::ostream& operator<<(std::ostream& os, Valve const& v)
{
    os << "{" << v.name << ", " << v.flow_rate << ", [";
    for (auto i = 0; i < v.valves_tunnels_to.size() - 1; i++)
        os << v.valves_tunnels_to[i] << ", ";
    os << v.valves_tunnels_to[v.valves_tunnels_to.size() - 1] << "]}";
    return os;
}

std::ostream& operator<<(std::ostream& os, ValveMap const& vm)
{
    os << "{\n";
    for (auto const& v : vm)
        os << '\t' << v.second << '\n';
    os << '}';
    return os;
}

ValveMap parse_input(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    std::string num{};
    std::string valve_name{};
    std::vector<std::string> valve_names;
    ValveMap valves;
    while (std::getline(ifs, line))
    {
        num.clear();
        valve_name.clear();
        valve_names.clear();
        for (auto c : line.substr(1))
            if (c >= '0' && c <= '9')
                num += c;
            else if (std::isupper(c))
            {
                valve_name += c;
                if (valve_name.size() == 2)
                {
                    valve_names.push_back(valve_name);
                    valve_name.clear();
                }
            }
        std::vector<std::string> valves_tunnels_to(valve_names.size() - 1);
        std::copy(valve_names.begin() + 1, valve_names.end(), valves_tunnels_to.begin());
        valves[valve_names[0]] = { valve_names[0], std::stoi(num), valves_tunnels_to };
    }
    return valves;
}

struct State
{
    int remained_time = 30;
    long released_pressure = 0;
    std::string current_valve;
    std::unordered_set<std::string> opened_valves;

    bool operator==(State const& other) const
    {
        return released_pressure == other.released_pressure && current_valve == other.current_valve;
    }

    struct Hasher
    {
        size_t operator()(State const& s) const
        {
            return std::hash<int>()(s.released_pressure) ^ std::hash<std::string>()(s.current_valve);
        }
    };
};

using StateSet = std::unordered_set<State, State::Hasher>;
using StateQueue = std::deque<State>;

long part_1(char const* fn)
{
    auto valves = parse_input(fn);
    //std::cout << valves << '\n';

    long max_pressure = 0;
    StateSet visited;
    StateQueue states;
    states.push_back({ 30, 0, "AA" });
    while (!states.empty())
    {
        auto current_state = states.front();
        states.pop_front();
        if (auto [_, yes] = visited.insert(current_state); yes)
        {
            max_pressure = std::max({ max_pressure, current_state.released_pressure });
            if (current_state.opened_valves.find(current_state.current_valve) == current_state.opened_valves.end() &&
                valves[current_state.current_valve].flow_rate > 0)
            {
                State new_state = current_state;
                new_state.remained_time -= 1;
                new_state.released_pressure += valves[new_state.current_valve].flow_rate * new_state.remained_time;
                new_state.opened_valves.insert(current_state.current_valve);
                states.push_back(new_state);
            }
            for (auto const& v : valves[current_state.current_valve].valves_tunnels_to)
            {
                State new_state = current_state;
                new_state.remained_time -= 1;
                new_state.current_valve = v;
                states.push_back(new_state);
            }
        }
    }
    
    return max_pressure;
}

struct StateWithElephant
{
    int remained_time = 26;
    long released_pressure = 0;
    std::string current_valve_1, current_valve_2;
    bool elephant_go = false;
    std::unordered_set<std::string> opened_valves;

    bool operator==(StateWithElephant const& other) const
    {
        return released_pressure == other.released_pressure && current_valve_1 == other.current_valve_1 && current_valve_2 == other.current_valve_2;
    }

    struct Hasher
    {
        size_t operator()(StateWithElephant const& s) const
        {
            return std::hash<int>()(s.released_pressure) ^ std::hash<std::string>()(s.current_valve_1) ^ std::hash<std::string>()(s.current_valve_2);
        }
    };
};

using StateWithElephantSet = std::unordered_set<StateWithElephant, StateWithElephant::Hasher>;
using StateWithElephantQueue = std::deque<StateWithElephant>;

long part_2(char const* fn)
{
    auto valves = parse_input(fn);

    long max_pressure = 0;
    StateWithElephantSet visited;
    StateWithElephantQueue states;
    states.push_back({ 26, 0, "AA", "AA" });
    int total_valves_can_open = std::reduce(valves.begin(), valves.end(), 0, [](int x, ValveMap::value_type const& p){ return p.second.flow_rate > 0 ? x + 1 : x; });
    auto get_pressure_from_valves_can_open = [&valves](StateWithElephant const& state)
    {
        return std::reduce(valves.begin(), valves.end(), 0,
            [&state](int x, ValveMap::value_type const& p)
            { return state.opened_valves.find(p.first) == state.opened_valves.end() ? x + p.second.flow_rate : x; })
            * (state.remained_time - 1)
            + state.released_pressure;
    };
    while (!states.empty())
    {
        auto current_state = states.front();
        states.pop_front();
        if (auto [_, yes] = visited.insert(current_state); yes)
        {
            if (current_state.released_pressure > max_pressure)
                max_pressure = current_state.released_pressure;
            if (current_state.remained_time == 0 && current_state.opened_valves.size() < total_valves_can_open)
                continue;
            if (get_pressure_from_valves_can_open(current_state) <= max_pressure)
                continue;
            if (!current_state.elephant_go)
            {
                if (current_state.opened_valves.find(current_state.current_valve_1) == current_state.opened_valves.end() &&
                valves[current_state.current_valve_1].flow_rate > 0)
                {
                    StateWithElephant new_state = current_state;
                    new_state.elephant_go = true;
                    new_state.released_pressure += valves[new_state.current_valve_1].flow_rate * (new_state.remained_time - 1);
                    new_state.opened_valves.insert(current_state.current_valve_1);
                    states.push_back(new_state);
                }
                for (auto const& v : valves[current_state.current_valve_1].valves_tunnels_to)
                {
                    StateWithElephant new_state = current_state;
                    new_state.elephant_go = true;
                    new_state.current_valve_1 = v;
                    states.push_back(new_state);
                }
            }
            else
            {
                if (current_state.opened_valves.find(current_state.current_valve_2) == current_state.opened_valves.end() &&
                valves[current_state.current_valve_2].flow_rate > 0)
                {
                    StateWithElephant new_state = current_state;
                    new_state.elephant_go = false;
                    new_state.remained_time -= 1;
                    new_state.released_pressure += valves[new_state.current_valve_2].flow_rate * new_state.remained_time;
                    new_state.opened_valves.insert(current_state.current_valve_2);
                    states.push_back(new_state);
                }
                for (auto const& v : valves[current_state.current_valve_2].valves_tunnels_to)
                {
                    StateWithElephant new_state = current_state;
                    new_state.elephant_go = false;
                    new_state.remained_time -= 1;
                    new_state.current_valve_2 = v;
                    states.push_back(new_state);
                }
            }
        }
    }
    
    return max_pressure;
}


int main(int argc, char* argv[])
{
    
    auto p = part_1(argv[1]);
    std::cout << "What is the most pressure you can release?" << '\n'
              << p << std::endl;

    // it takes almost 1 mins and 5GB memory...
    auto s = part_2(argv[1]);
    std::cout << "With you and an elephant working together for 26 minutes, what is the most pressure you could release?" << '\n'
              << s << std::endl;

    return 0;
}