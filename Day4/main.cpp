#include <iostream>
#include <fstream>
#include <cstring>

namespace Day4 {
    struct Range {
        int start;
        int end;

        explicit Range(const std::string& range) {
            size_t dash_pos = range.find('-');
            start = std::stoi(range.substr(0, dash_pos));
            end = std::stoi(range.substr(dash_pos + 1, range.length()));
        }
    };

    static bool does_range_full_contain_other(const Range& first, const Range& second) {
        if (first.start <= second.start && first.end >= second.end) {
            return true;
        }
        return false;
    }

    static bool does_range_overlap_other(const Range& first, const Range& second) {
        if (first.start <= second.end && first.start >= second.start) {
            return true;
        }
        if (first.end >= second.start && first.end <= second.end) {
            return true;
        }

        return false;
    }

    static void part1(std::string_view sv) {
        std::ifstream file(sv.data());

        int result = 0;
        for (std::string line; std::getline(file, line);) {
            size_t comma_pos = line.find(',');
            Range first_elf(line.substr(0, comma_pos));
            Range second_elf(line.substr(comma_pos + 1, line.length()));
            if (does_range_full_contain_other(first_elf, second_elf) || does_range_full_contain_other(second_elf, first_elf)) {
                result++;
            }
        }
        printf("Part1: %d\n", result);
    }

    static void part2(std::string_view sv) {
        std::ifstream file(sv.data());

        int result = 0;
        for (std::string line; std::getline(file, line);) {
            size_t comma_pos = line.find(',');
            Range first_elf(line.substr(0, comma_pos));
            Range second_elf(line.substr(comma_pos + 1, line.length()));
            if (does_range_overlap_other(first_elf, second_elf) || does_range_overlap_other(second_elf, first_elf)) {
                result++;
            }
        }
        printf("Part2: %d\n", result);
    }
}

int main() {
    Day4::part1("Input1.txt");
    Day4::part2("Input1.txt");
    return 0;
}
