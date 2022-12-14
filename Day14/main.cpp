#include <iostream>
#include <fstream>
#include <vector>
#include <functional>

namespace Day14 {
    typedef std::pair<int, int> Coordinates;


    static std::vector<std::pair<Coordinates, Coordinates>> read_lines(std::string_view sv) {
        std::ifstream file(sv.data());
        std::vector<std::pair<Coordinates, Coordinates>> lines;
        std::string line;
        while (getline(file, line)) {
            std::vector<int> numbers;
            int tmp = 0;
            for (char& c: line) {
                if (c >= '0' && c <= '9') {
                    tmp = tmp * 10 + (c - '0');
                } else {
                    if (tmp > 0) {
                        numbers.push_back(tmp);
                        tmp = 0;
                    }
                }
            }
            if (tmp > 0) {
                numbers.push_back(tmp);
            }

            for (int i = 3; i < (int) numbers.size(); i += 2) {
                lines.emplace_back(std::make_pair(numbers[i - 3], numbers[i - 2]),
                                   std::make_pair(numbers[i - 1], numbers[i]));
            }
        }
        return lines;
    }

    static void part1(std::string_view sv) {
        std::vector<std::pair<Coordinates, Coordinates>> lines = std::move(read_lines(sv));

        char grid[600][600];

        for (int i = 0; i < 600; i++) {
            for (int j = 0; j < 600; j++) {
                grid[i][j] = '.';
            }
        }

        int maxY = 0;

        for (auto& line: lines) {
            auto& start = line.first;
            auto& end = line.second;

            maxY = std::max(maxY, std::max(start.second, end.second));

            if (start.first == end.first) {
                auto l = std::minmax(start.second, end.second);
                for (int i = l.first; i <= l.second; i++) {
                    grid[i][start.first] = '#';
                }
            } else {
                auto l = std::minmax(start.first, end.first);
                for (int i = l.first; i <= l.second; i++) {
                    grid[start.second][i] = '#';
                }
            }
        }

        int count = 0;

        Coordinates sand = {500, 0};

        while (true) {
            if (sand.second > maxY) {
                break;
            }

            if (grid[sand.second + 1][sand.first] == '.') {
                sand.second++;
            } else if (grid[sand.second + 1][sand.first - 1] == '.') {
                sand.second++;
                sand.first--;
            } else if (grid[sand.second + 1][sand.first + 1] == '.') {
                sand.second++;
                sand.first++;
            } else {
                count++;
                grid[sand.second][sand.first] = 'o';
                sand = {500, 0};
            }
        }
        printf("Part1: %d\n", count);
    }

    static void part2(std::string_view sv) {
        std::vector<std::pair<Coordinates, Coordinates>> lines = std::move(read_lines(sv));

        char grid[600][600];

        for (int i = 0; i < 600; i++) {
            for (int j = 0; j < 600; j++) {
                grid[i][j] = '.';
            }
        }

        int maxY = 0;

        for (auto& line: lines) {
            auto& start = line.first;
            auto& end = line.second;

            maxY = std::max(maxY, std::max(start.second, end.second));

            if (start.first == end.first) {
                auto l = std::minmax(start.second, end.second);
                for (int i = l.first; i <= l.second; i++) {
                    grid[i][start.first] = '#';
                }
            } else {
                auto l = std::minmax(start.first, end.first);
                for (int i = l.first; i <= l.second; i++) {
                    grid[start.second][i] = '#';
                }
            }
        }

        int count = 0;

        Coordinates sand = {500, 0};

        while (true) {

            if (sand.second == (maxY + 1)) {
                count++;
                grid[sand.second][sand.first] = 'o';
                sand = {500, 0};
            } else if (grid[sand.second + 1][sand.first] == '.') {
                sand.second++;
            } else if (grid[sand.second + 1][sand.first - 1] == '.') {
                sand.second++;
                sand.first--;
            } else if (grid[sand.second + 1][sand.first + 1] == '.') {
                sand.second++;
                sand.first++;
            } else {
                count++;
                if (sand.first == 500 && sand.second == 0) {
                    break;
                }
                grid[sand.second][sand.first] = 'o';
                sand = {500, 0};
            }
        }
        printf("Part2: %d\n", count);
    }
}

int main() {
    Day14::part1("Input1.txt");
    Day14::part2("Input1.txt");
}
