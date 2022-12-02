#include <iostream>
#include <fstream>
#include <string_view>

#define NUM_MAXES 3

namespace Day1 {
    static void part1(std::string_view sv) {
        std::ifstream file(sv.begin());

        int max = 0;
        int current = 0;
        for (std::string line; std::getline(file, line);) {
            if (line.empty()) {
                max = std::max(current, max);
                current = 0;
            } else {
                current += std::stoi(line);
            }
        }
        max = std::max(current, max);
        printf("Part1: %d\n", max);
    }

    static void shift_maxes(int* current, int* maxes) {
        for (int i = 0; i < NUM_MAXES; i++) {
            if (*current > maxes[i]) {
                auto tmp = maxes[i];
                maxes[i] = *current;
                *current = tmp;
            }
        }
    }

    static void part2(std::string_view sv) {
        std::ifstream file(sv.begin());

        int maxes[NUM_MAXES] = {0};
        int current = 0;
        for (std::string line; std::getline(file, line);) {
            if (line.empty()) {
                shift_maxes(&current, maxes);
                current = 0;
            } else {
                current += std::stoi(line);
                if (file.eof()) {
                    shift_maxes(&current, maxes);
                }
            }
        }

        int result = 0;
        for (auto& max: maxes) {
            result += max;
        }
        printf("Part2: %d\n", result);
    }
}

int main() {
    std::string file_name = "Input1.txt";
    Day1::part1(file_name);
    Day1::part2(file_name);
    return 0;
}