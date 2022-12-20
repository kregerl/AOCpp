#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "SimpleParser.h"

namespace Day20 {
    std::vector<int64_t> data{};
    std::vector<int64_t> position{};

    int64_t dataAt(const int64_t pos) {
        for (int i = 0; i < (int64_t)position.size(); i++) {
            if (position[i] == pos % (int64_t) position.size())
                return data[i];
        }
        printf("dataAt(%ld)\n", pos);
        throw ("not reached");
    }

    void printData() {
        printf("%ld", dataAt(0));
        for (int i = 1; i < position.size(); i++) {
            printf(", %ld", dataAt(position[i]));
        }
        printf("\n");
    }

    void mix() {
        const auto modulus = (int64_t) data.size();
        for (int i = 0; i < modulus; i++) {
            if (data[i] % (modulus - 1) == 0)
                continue;
            const auto oldPos = position[i];
            const auto newRawPos = oldPos + data[i] % (modulus - 1);
            const auto newPos = [&]() {
                if (0 < newRawPos and newRawPos < modulus)
                    return newRawPos;
                if (newRawPos <= 0)
                    return newRawPos + modulus - 1;
                return newRawPos - modulus + 1;
            }();
            for (auto& ele: position) {
                if (oldPos < newPos and oldPos <= ele and ele <= newPos) {
                    ele = (ele - 1 + modulus) % modulus;
                } else if (newPos < oldPos and newPos <= ele and ele <= oldPos) {
                    ele = (ele + 1) % modulus;
                }
            }
            position[i] = newPos;
        }
    }

    static int64_t part_impl(std::string_view sv, int64_t decryption_key = 1) {
        position.clear();
        data.clear();
        SimpleParser scanner{sv.data()};
        int64_t index = 0;
        int64_t zeroPos = 0;
        while (!scanner.isEof()) {
            data.push_back(scanner.getInt64() * decryption_key);
            if (data.back() == 0) {
                zeroPos = index;
            }
            position.push_back(index);
            ++index;
        }
        mix();
        if (decryption_key != 1) {
            for (int count = 0; count < 9; count++) {
                mix();
            }
        }
        const auto d1000 = dataAt(1000 + position[zeroPos]);
        const auto d2000 = dataAt(2000 + position[zeroPos]);
        const auto d3000 = dataAt(3000 + position[zeroPos]);
        return d1000 + d2000 + d3000;
    }

    static void part1(std::string_view sv) {
        auto result = part_impl(sv);
        printf("Part1: %ld\n", result);
    }

    static void part2(std::string_view sv) {
        auto result = part_impl(sv, 811589153);
        printf("Part2: %ld\n", result);
    }
}

int main() {
    Day20::part1("Input1.txt");
    Day20::part2("Input1.txt");
}