#include <iostream>
#include <array>
#include <cassert>
#include <deque>
#include <fstream>
#include <functional>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <tuple>

namespace Day18 {
    using Coordinate = std::tuple<std::int32_t, std::int32_t, std::int32_t>;

    constexpr static auto s_directions = std::array<Coordinate, 6>{
            Coordinate{1, 0, 0},
            Coordinate{-1, 0, 0},
            Coordinate{0, 1, 0},
            Coordinate{0, -1, 0},
            Coordinate{0, 0, 1},
            Coordinate{0, 0, -1}
    };

    auto operator+(const Coordinate& a_Left, const Coordinate& a_Right) {
        return Coordinate{(std::get<0>(a_Left) + std::get<0>(a_Right)), (std::get<1>(a_Left) + std::get<1>(a_Right)),
                          (std::get<2>(a_Left) + std::get<2>(a_Right))};
    }

    bool InRange(const Coordinate& a_Input) {
        return (
                (std::get<0>(a_Input) >= -1) && (std::get<0>(a_Input) <= 21) &&
                (std::get<1>(a_Input) >= -1) && (std::get<1>(a_Input) <= 21) &&
                (std::get<2>(a_Input) >= -1) && (std::get<2>(a_Input) <= 21));
    }

    void Part1(std::string_view sv) {
        std::ifstream file{sv.data()};
        std::string line;

        std::set<Coordinate> droplets;
        while (std::getline(file, line)) {
            auto input = std::stringstream{line};
            std::string x;
            auto parsed_x = static_cast<bool>(std::getline(input, x, ','));
            assert(parsed_x);
            std::string y;
            auto parsed_y = static_cast<bool>(std::getline(input, y, ','));
            assert(parsed_y);
            std::string z;
            auto parsed_z = static_cast<bool>(std::getline(input, z, ','));
            assert(parsed_z);
            droplets.insert(Coordinate{std::atoi(x.c_str()), std::atoi(y.c_str()), std::atoi(z.c_str())});
        }

        auto water = std::set<Coordinate>{};
        auto test_coordinates = std::set<Coordinate>{};
        test_coordinates.insert(Coordinate{0, 0, 0});
        while (!test_coordinates.empty()) {
            auto next_test_coords = std::set<Coordinate>{};
            for (const auto& test_coordinate: test_coordinates) {
                if (InRange(test_coordinate) &&
                    (!water.count(test_coordinate) && (!droplets.count(test_coordinate)))) {
                    water.insert(test_coordinate);
                    for (const auto& direction: s_directions) {
                        next_test_coords.insert(test_coordinate + direction);
                    }
                }
            }

            test_coordinates = next_test_coords;
        }

        auto exposed_verts = 0;
        for (const auto& l_Droplet: droplets) {
            for (const auto& l_Direction: s_directions) {
                if (!droplets.count(l_Droplet + l_Direction)) {
                    // neighbor is no droplet, must be "air" or "water": count for riddle A
                    ++exposed_verts;
                }
            }
        }

        printf("Part1: %d\n", exposed_verts);
    }

    void Part2(std::string_view sv) {
        std::ifstream file{sv.data()};
        std::string line;

        std::set<Coordinate> droplets;
        while (std::getline(file, line)) {
            auto input = std::stringstream{line};
            std::string x;
            auto parsed_x = static_cast<bool>(std::getline(input, x, ','));
            assert(parsed_x);
            std::string y;
            auto parsed_y = static_cast<bool>(std::getline(input, y, ','));
            assert(parsed_y);
            std::string z;
            auto parsed_z = static_cast<bool>(std::getline(input, z, ','));
            assert(parsed_z);
            droplets.insert(Coordinate{std::atoi(x.c_str()), std::atoi(y.c_str()), std::atoi(z.c_str())});
        }

        auto water = std::set<Coordinate>{};
        auto test_coordinates = std::set<Coordinate>{};
        test_coordinates.insert(Coordinate{0, 0, 0});
        while (!test_coordinates.empty()) {
            auto next_test_coords = std::set<Coordinate>{};
            for (const auto& test_coordinate: test_coordinates) {
                if (InRange(test_coordinate) &&
                    (!water.count(test_coordinate) && (!droplets.count(test_coordinate)))) {
                    water.insert(test_coordinate);
                    for (const auto& direction: s_directions) {
                        next_test_coords.insert(test_coordinate + direction);
                    }
                }
            }

            test_coordinates = next_test_coords;
        }

        auto exposed_verts = 0;
        for (const auto& l_Droplet: droplets) {
            for (const auto& l_Direction: s_directions) {
                if (water.count(l_Droplet + l_Direction)) {
                    // neighbor is water: count for riddle B
                    ++exposed_verts;
                }
            }
        }

        printf("Part2: %d\n", exposed_verts);
    }
}

int main() {
    Day18::Part1("Input1.txt");
    Day18::Part2("Input1.txt");
}