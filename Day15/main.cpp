#include <iostream>
#include <string_view>
#include <vector>
#include <utility>
#include <fstream>
#include <string>
#include <algorithm>

namespace Day15 {
    static std::vector<std::pair<std::pair<int, int>, int>> read_sensors(std::string_view sv) {
        std::ifstream file(sv.data());
        std::vector<std::pair<std::pair<int, int>, int>> sensors;

        for (std::string line; std::getline(file, line);) {
            std::vector<int> numbers;
            int tmp = 0;
            int sign = 1;
            bool found = false;
            for (char &c: line) {
                if (c >= '0' && c <= '9') {
                    tmp = tmp * 10 + (c - '0');
                    found = true;
                } else if (c == '-') {
                    sign = -1;
                } else {
                    if (found) {
                        numbers.push_back(tmp * sign);
                        tmp = 0;
                        sign = 1;
                        found = false;
                    }
                }
            }
            if (found) {
                numbers.push_back(tmp * sign);
            }

            int x1 = numbers[0];
            int y1 = numbers[1];
            int x2 = numbers[2];
            int y2 = numbers[3];

            sensors.emplace_back(std::make_pair(y1, x1), abs(x1 - x2) + abs(y1 - y2));
        }
        return sensors;
    }

    static void part1(std::string_view sv) {
        std::vector<std::pair<std::pair<int, int>, int>> sensors = read_sensors(sv);
        std::vector<std::pair<int, int>> ranges;
        for (auto &s: sensors) {
            auto &coords = s.first;
            auto &dist = s.second;
            int side = dist - abs(coords.first - 2000000);

            if (side < 0)
                continue;

            ranges.emplace_back(coords.second - side, coords.second + side);
        }

        std::sort(ranges.begin(), ranges.end());

        int r = 0;

        for (int i = 1; i < (int) ranges.size(); i++) {
            if (ranges[r].second >= ranges[i].first) {
                ranges[r].second = std::max(ranges[r].second, ranges[i].second);
            } else {
                r++;
                ranges[r] = ranges[i];
            }
        }

        int result = 0;
        for (int i = 0; i <= r; i++) {
            result += (ranges[i].second - ranges[i].first) + 1;
        }

        printf("Part1: %d\n", result - 1);
    }

    // Kinds sucks cause its slow but works.
    static void part2(std::string_view sv) {
        std::vector<std::pair<std::pair<int, int>, int>> sensors = read_sensors(sv);

        for (long long Y = 0; Y <= 4000000; Y++) {
            std::vector<std::pair<int, int>> ranges;

            for (auto &s: sensors) {
                auto &coords = s.first;
                auto &dist = s.second;
                long long side = dist - abs(coords.first - Y);

                if (side < 0)
                    continue;

                ranges.emplace_back(coords.second - side, coords.second + side);
            }

            std::sort(ranges.begin(), ranges.end());

            long long r = 0;

            for (int i = 1; i < (int) ranges.size(); i++) {
                if (ranges[r].second >= ranges[i].first) {
                    ranges[r].second = std::max(ranges[r].second, ranges[i].second);
                } else {
                    r++;
                    ranges[r] = ranges[i];
                }
            }

            if (r > 0) {
                long long X = ranges[0].second + 1;
                printf("Part2: %lld\n", (X * 4000000 + Y));
                return;
            }
        }
    }
}


int main() {
    Day15::part1("Input1.txt");
    Day15::part2("Input1.txt");
}