//
// Created by Harold on 2022/12/7.
//

#include <fstream>
#include <string>
#include <string_view>
#include <set>
#include <map>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

size_t part_1(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    std::set<std::string> dirs;
    std::map<std::string, size_t> sizes;
    fs::path current{};
    size_t sum = 0;
    while (std::getline(ifs, line))
    {
        std::string_view line_view(line);
        // commands
        if (line_view[0] == '$')
        {
            if (line_view.substr(2, 2) == "cd")
            {
                if (line_view[5] == '/')
                    current = fs::path("\\");
                else if (line_view.substr(5, 2) == "..")
                    current = current.parent_path();
                else
                    current = current / line_view.substr(5);
                dirs.insert(current.string());
            }
            // ls -> do nothing
        }
        // dirs and files
        else
        {
            // file
            if (line_view[0] != 'd')
            {
                auto space_pos = line_view.find(' ');
                sizes[(current / line_view.substr(space_pos + 1)).string()] = std::stoull(line.substr(0, space_pos));
            }
        }
    }

    for (auto const& dir : dirs)
    {
        size_t dir_size = 0;
        for (auto const& [path, sz] : sizes)
            if (path.rfind(dir, 0) != std::string::npos)
                dir_size += sz;
        if (dir_size <= 100000)
            sum += dir_size;
        //std::cout << dir << " : " << dir_size << '\n';
    }
    
    return sum;
}

size_t part_2(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    std::set<std::string> dirs;
    std::map<std::string, size_t> sizes;
    fs::path current{};
    size_t smallest = std::numeric_limits<size_t>::max();
    while (std::getline(ifs, line))
    {
        std::string_view line_view(line);
        // commands
        if (line_view[0] == '$')
        {
            if (line_view.substr(2, 2) == "cd")
            {
                if (line_view[5] == '/')
                    current = fs::path("\\");
                else if (line_view.substr(5, 2) == "..")
                    current = current.parent_path();
                else
                    current = current / line_view.substr(5, 2);
                dirs.insert(current.string());
            }
            // ls -> do nothing
        }
        // dirs and files
        else
        {
            // file
            if (line_view[0] != 'd')
            {
                auto space_pos = line_view.find(' ');
                sizes[(current / line_view.substr(space_pos + 1)).string()] = std::stoull(line.substr(0, space_pos));
            }
        }
    }

    size_t used = 0;
    for (auto const& [_, sz] : sizes)
        used += sz;
    size_t required = 30000000 - (70000000 - used);
    for (auto const& dir : dirs)
    {
        size_t dir_size = 0;
        for (auto const& [path, sz] : sizes)
            if (path.rfind(dir, 0) != std::string::npos)
                dir_size += sz;
        if (dir_size >= required && dir_size < smallest)
            smallest = dir_size;
    }
    
    return smallest;
}

int main(int argc, char* argv[])
{
    
    auto p = part_1(argv[1]);
    std::cout << "What is the sum of the total sizes of those directories?" << '\n'
              << p << std::endl;

    p = part_2(argv[1]);
    std::cout << "What is the total size of that directory?" << '\n'
              << p << std::endl;

    return 0;
}