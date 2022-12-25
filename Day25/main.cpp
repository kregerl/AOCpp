#include <iostream> 
#include <string_view>
#include <fstream>
#include <complex>
#include <unordered_map>

namespace Day25 {
    static long snafu_to_decimal(std::string number) {
        long decimal = 0L;
        for (int i = 0; i < number.length(); i++) {
            char c = number[i];
            if (std::isdigit(c)) {
                decimal += (c - '0') * std::pow(5.0, number.length() - i - 1);
            } else if (c == '-') {
                decimal += -1 * std::pow(5.0, number.length() - i - 1);
            } else if (c == '=') {
                decimal += -2 * std::pow(5.0, number.length() - i - 1);
            }
        }
        return decimal;
    }

    std::unordered_map<long, char> snafu_mappings = {
            {-2L, '='},
            {-1L, '-'},
            {0L, '0'},
            {1L, '1'},
            {2L, '2'},
    };

    static std::string decimal_to_snafu(long number) {
        std::stringstream ss;
        while (number > 0) {
            auto place = (number + 2) % 5;
            auto snafu_char = snafu_mappings.at(place - 2);
            ss << snafu_char;
            number = (number + 2) / 5;
        }
        auto result = ss.str();
        std::reverse(result.begin(), result.end());
        return result;
    }

    static void part1(std::string_view sv) {
        std::ifstream file(sv.data());

        long sum = 0;
        for (std::string line; std::getline(file, line);) {
            auto decimal = snafu_to_decimal(line);
            sum += decimal;
        }
        printf("Part1: %ld\n", sum);
        std::cout << decimal_to_snafu(sum) << std::endl;
    }
}


int main() {
    Day25::part1("Input1.txt");
}
