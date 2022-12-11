#include <iostream>
#include <string_view>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

namespace Day11 {
    enum class Operation {
        Addition = 0,
        Multiplication
    };


    struct Monkey {
        int id;
        std::vector<int> items;
        Operation op;
        std::string op_value;
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
            monkey.op = op;
            monkey.op_value = chunk[2].substr(pos + 2);

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
        std::vector<int> monkey_inspections(monkeys.size());

        for (int i = 0; i < 20; i++) {
            for (auto& monkey : monkeys) {
                for (int j = 0; j < monkey.items.size(); j++) {
                    auto item = monkey.items[j];
                    int64_t worry_level;
                    switch (monkey.op) {
                        case Operation::Multiplication: {
                            if (monkey.op_value == "old") {
                                worry_level = item * item;
                            } else {
                                worry_level = std::stoi(monkey.op_value) * item;
                            }
                            break;
                        }
                        case Operation::Addition: {
                            if (monkey.op_value == "old") {
                                worry_level = item * item;
                            } else {
                                worry_level = std::stoi(monkey.op_value) + item;
                            }
                            break;
                        }
                    }
                    worry_level /= 3;
                    if (worry_level % monkey.divisible_by == 0) {
                        monkeys.at(monkey.if_true_id).items.emplace_back(worry_level);
                    } else {
                        monkeys.at(monkey.if_false_id).items.emplace_back(worry_level);
                    }
                    monkey_inspections[monkey.id]++;
                }
                monkey.items.clear();
            }
        }
        int result = 0;
        auto max = std::max_element(monkey_inspections.begin(), monkey_inspections.end());
        result = *max;
        monkey_inspections.erase(max);
        auto max2 = std::max_element(monkey_inspections.begin(), monkey_inspections.end());

        result *= *max2;
        printf("Part1: %d\n", result);
    }

    static void part2(std::string_view sv) {
        std::vector<Monkey> monkeys = read_monkeys(sv);
        std::vector<int> monkey_inspections(monkeys.size());

        for (int i = 0; i < 20; i++) {
            for (auto& monkey : monkeys) {
                for (int j = 0; j < monkey.items.size(); j++) {
                    auto item = monkey.items[j];
                    int64_t worry_level;
                    switch (monkey.op) {
                        case Operation::Multiplication: {
                            if (monkey.op_value == "old") {
                                worry_level = item * item;
                            } else {
                                worry_level = std::stoi(monkey.op_value) * item;
                            }
                            break;
                        }
                        case Operation::Addition: {
                            worry_level = std::stoi(monkey.op_value) + item;
                            break;
                        }
                    }
                    if (worry_level % monkey.divisible_by == 0) {
                        monkeys.at(monkey.if_true_id).items.emplace_back(worry_level);
                    } else {
                        monkeys.at(monkey.if_false_id).items.emplace_back(worry_level);
                    }
                    monkey_inspections[monkey.id]++;
                }
                monkey.items.clear();
            }
        }
        int result = 0;
        auto max = std::max_element(monkey_inspections.begin(), monkey_inspections.end());
        result = *max;
        monkey_inspections.erase(max);
        auto max2 = std::max_element(monkey_inspections.begin(), monkey_inspections.end());

        result *= *max2;
        printf("Part2: %d\n", result);
    }
}


int main() {
    Day11::part1("Input1.txt");
    Day11::part2("Example.txt");
}
