#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

namespace Day13 {
    static bool is_number(const char c) {
        return c >= '0' && c <= '9';
    }

    static bool compare(const std::string& packet1, const std::string& packet2) {
        int i1 = 0;
        int i2 = 0;
        while (i1 < packet1.size() && i2 < packet2.size()) {
            if (is_number(packet1[i1]) && is_number(packet2[i2])) {
                int n1 = 0;
                while (is_number(packet1[i1])) {
                    n1 = n1 * 10 + (packet1[i1] - '0');
                    i1++;
                }
                int n2 = 0;
                while (is_number(packet2[i2])) {
                    n2 = n2 * 10 + (packet2[i2] - '0');
                    i2++;
                }
                if (n1 == n2) {

                    continue;
                }
                if (n1 < n2) {

                    return true;
                }
                break;
            } else if (packet1[i1] == packet2[i2]) {
                i1++;
                i2++;
            } else if (packet1[i1] == ']') {
                return true;
            } else if (packet2[i2] == ']') {
                break;
            } else if (packet1[i1] == '[' || packet1[i1] == ',') {
                i1++;
            } else if (packet2[i2] == ',' || packet2[i2] == '[') {
                i2++;
            }
        }
        if (i1 == packet1.size()) {

            return true;
        }
        return false;
    }

    static void part1(std::string_view sv) {
        std::string packet1, packet2;
        std::fstream file(sv.data());

        int index = 0;
        int result = 0;

        while (std::getline(file, packet1)) {
            if (packet1.empty()) continue;
            std::getline(file, packet2);
            index++;
            if (compare(packet1, packet2)) {
                result += index;
            }
        }

        printf("Part1: %d\n", result);
    }

    static void part2(std::string_view sv) {
        std::string line;
        std::fstream file(sv.data());
        std::vector<std::string> packets;
        while(std::getline(file, line)) {
            if (line.empty()) continue;
            packets.push_back(line);
        }

        const std::vector<std::string> divider_packets {
                "[[2]]",
                "[[6]]"
        };

        for (const auto& divider_packet : divider_packets) {
            packets.push_back(divider_packet);
        }

        std::sort(std::begin(packets), std::end(packets), compare);

        int result = 1;
        for (const auto& divider_packet : divider_packets) {
            result *= (std::distance(std::begin(packets), std::find(std::begin(packets), std::end(packets), divider_packet)) + 1);
        }

        printf("Part2: %d\n", result);
    }
}



int main() {
    Day13::part1("Input1.txt");
    Day13::part2("Input1.txt");
    return 0;
}
