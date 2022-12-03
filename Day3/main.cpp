#include <iostream>
#include <fstream>

namespace Day3 {

    static int get_priority(char c) {
        if (std::islower(c)) {
            return (c - 'a') + 1;
        } else {
            return 27 + (c - 'A');
        }
    }

    static int find_in_compartments(const std::string& rucksack) {
        std::string first_compartment = rucksack.substr(0, rucksack.length() / 2);
        std::string second_compartment = rucksack.substr(rucksack.length() / 2);
        if (first_compartment.length() != second_compartment.length()) {
            std::cout << "Compartments not the same length" << std::endl;
        }
        for (auto& first: first_compartment) {
            for (auto& second: second_compartment) {
                if (first == second) {
                    return get_priority(first);
                }
            }
        }
        return 0;
    }

    void part1(std::string_view sv) {
        std::ifstream file(sv.data());
        int result = 0;
        for (std::string line; std::getline(file, line);) {
            result += find_in_compartments(line);
        }
        printf("Part1: %d\n", result);
    }

    // This is not a good solution but it works for small enough inputs.
    static int find_badge(const std::string& first, const std::string& second, const std::string& third) {
        for (auto& f: first) {
            for (auto& s: second) {
                for (auto& t: third) {
                    if ((f == s) && (s == t)) {
                        return get_priority(f);
                    }
                }
            }
        }
        return 0;
    }

    void part2(std::string_view sv) {
        std::ifstream file(sv.data());
        std::string first, second, third;
        int result = 0;
        while (file >> first >> second >> third) {
            result += find_badge(first, second, third);
        }
        printf("Part2: %d\n", result);
    }
}

int main() {
    Day3::part1("Input1.txt");
    Day3::part2("Input1.txt");
    return 0;
}
