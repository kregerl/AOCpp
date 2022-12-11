#include <iostream>
#include <string_view>
#include <fstream>
#include <vector>
#include <sstream>

namespace Day11 {
    enum class Operation {
        Addition = 0,
        Multiplication
    };


    struct Monkey {
        int id;
        std::vector<int> items;
        Operation op;
        int op_value;
        int divisible_by;
        int if_true_id;
        int if_false_id;
    };

    static std::vector<Monkey> read_monkeys(std::string_view sv) {
        std::ifstream file(sv.data());

        std::vector<std::string> lines;
        for (std::string line; std::getline(file, line);) {
            lines.emplace_back(line);
        }
        size_t chunk_size = 7;
        std::vector<std::vector<std::string>> chunks;
        for (size_t i = 0; i < lines.size(); i += chunk_size) {
            auto last = std::min(lines.size(), i + chunk_size);
            chunks.emplace_back(lines.begin() + i, lines.begin() + last);
        }
        std::vector<Monkey> monkeys;
        for (auto& chunk: chunks) {
            Monkey monkey;
            monkey.id = chunk[0][chunk[0].size() - 2] - '0';
            auto monkey_items = chunk[1].substr(chunk[1].find(':') + 2);
            std::vector<int> items;
            std::stringstream ss(monkey_items);
            while (ss.good()) {
                std::string substr;
                std::getline(ss, substr, ',');
                items.emplace_back(std::stoi(substr));
            }
            monkey.items = std::move(items);
            size_t pos = chunk[2].find('+');
            if (pos == std::string::npos) {
                pos = chunk[2].find('*');
            }
            auto op = chunk[2].substr(pos, 1) == "+" ? Operation::Addition : Operation::Multiplication;
            auto op_value = std::stoi(chunk[2].substr(pos + 2));
            monkey.op = op;
            monkey.op_value = op_value;

            std::string by = "by";
            auto divisible_by = std::stoi(chunk[3].substr(chunk[3].find(by) + by.size() +  1));
            monkey.divisible_by = divisible_by;

            std::string mnky = "monkey";
            auto true_monkey_id = std::stoi(chunk[4].substr(chunk[4].find(mnky) + mnky.size() + 1));
            auto false_monkey_id = std::stoi(chunk[5].substr(chunk[5].find(mnky) + mnky.size() + 1));
            monkey.if_true_id = true_monkey_id;
            monkey.if_false_id = false_monkey_id;
            monkeys.emplace_back(monkey);
        }
        return monkeys;
    }

    static void part1(std::string_view sv) {
        std::vector<Monkey> monkeys = read_monkeys(sv);


    }
}


int main() {
    Day11::part1("Example.txt");
}
