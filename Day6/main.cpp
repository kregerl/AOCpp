#include <iostream>
#include <string_view>
#include <vector>
#include <fstream>
#include <algorithm>

namespace Day6 {

    static int part_impl(std::string_view sv, int unique_chars_to_find) {
        std::ifstream file(sv.data());

        std::string line;
        file >> line;

        for (int i = 0; i < line.length(); i++) {
            std::vector<char> seen;
            for (int j = i + 1; j < line.length(); j++) {
                if (seen.size() >= unique_chars_to_find) {
                    return j;
                }
                auto current = line[i];
                auto next = line[j];

                auto is_not_duplicate = current != next && std::find(seen.begin(), seen.end(), next) == seen.end();
                if (is_not_duplicate) {
                    seen.emplace_back(line[j]);
                } else {
                    break;
                }
            }
        }
        return -1;
    }

    // 13 chars + the current = 14 total uniques in a row
    static void part2(std::string_view sv) {
        auto result = part_impl(sv, 13);
        printf("Part2: %d\n", result);
    }

    static void part1(std::string_view sv) {
        // 3 chars + the current = 4 total uniques in a row
        auto result = part_impl(sv, 3);
        printf("Part2: %d\n", result);
    }
}

int main() {
    Day6::part1("Input1.txt");
    Day6::part2("Input1.txt");
}