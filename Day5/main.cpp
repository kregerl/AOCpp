#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <sstream>

#define MAX_STACKS 10

namespace Day5 {
    struct Instruction {
        int amount;
        int from_index;
        int to_index;

        explicit Instruction(const std::string& instruction) {
            std::stringstream ss;
            ss << instruction;

            std::string tmp;
            int found, idx = 0;
            while (!ss.eof()) {
                ss >> tmp;

                if (std::stringstream(tmp) >> found) {
                    if (idx == 0) {
                        amount = found;
                    } else if (idx == 1) {
                        from_index = found - 1;
                    } else if (idx == 2) {
                        to_index = found - 1;
                    } else {
                        std::cout << "Not possible" << std::endl;
                    }
                    idx++;
                }
            }
        }
    };

    static void print_tops(std::string_view part, const std::vector<std::stack<std::string>>& stacks) {
        std::stringstream ss;
        for (auto& stack : stacks) {
            if (!stack.empty()) {
                ss <<  stack.top().substr(1, 1);
            }
        }
        printf("%s: %s\n", part.data(), ss.str().c_str());
    }

    static void perform_movement_part1(std::vector<std::stack<std::string>>* stacks, const Instruction& instruction) {
        auto from_stack = stacks->at(instruction.from_index);
        auto to_stack = stacks->at(instruction.to_index);
        for (int i = 0; i < instruction.amount && !from_stack.empty(); i++) {
            to_stack.push(from_stack.top());
            from_stack.pop();
        }
        stacks->at(instruction.from_index) = from_stack;
        stacks->at(instruction.to_index) = to_stack;
    }

    static void perform_movement_part2(std::vector<std::stack<std::string>>* stacks, const Instruction& instruction) {
        auto from_stack = stacks->at(instruction.from_index);
        auto to_stack = stacks->at(instruction.to_index);

        std::stack<std::string> tmp_stack;
        for (int i = 0; i < instruction.amount && !from_stack.empty(); i++) {
            tmp_stack.push(from_stack.top());
            from_stack.pop();
        }
        while (!tmp_stack.empty()) {
            to_stack.push(tmp_stack.top());
            tmp_stack.pop();
        }
        stacks->at(instruction.from_index) = from_stack;
        stacks->at(instruction.to_index) = to_stack;
    }

    static void flip_stacks(std::vector<std::stack<std::string>>* stacks) {
        for (int i = 0; i < MAX_STACKS; i++) {
            std::stack<std::string> tmp;
            auto stack = stacks->at(i);
            while (!stack.empty()) {
                tmp.push(stack.top());
                stack.pop();
            }
            stacks->at(i) = tmp;
        }
    }

    static void part_impl(std::string_view sv, std::string_view part) {
        std::ifstream file(sv.data());

        std::vector<std::stack<std::string>> stacks;
        stacks.resize(MAX_STACKS);
        bool is_reading_instructions = false;
        for (std::string line; std::getline(file, line);) {
            if (line.empty()) {
                is_reading_instructions = true;
                flip_stacks(&stacks);
                continue;
            }

            if (is_reading_instructions) {
                if (part == "part1") {
                    perform_movement_part1(&stacks, Instruction(line));
                } else {
                    perform_movement_part2(&stacks, Instruction(line));
                }
            } else {
                int i = 0;
                if (line.find('[') == std::string::npos)
                    continue;
                while (i < line.length()) {
                    int idx = (i / 4);
                    std::string sub = line.substr(i, 3);
                    if (sub.find_first_not_of(' ') != std::string::npos)
                        stacks.at(idx).push(sub);
                    i += 4;
                }
            }

        }
        print_tops(part, stacks);
    }

    static void part1(std::string_view sv) {
        part_impl(sv, "part1");
    }

    static void part2(std::string_view sv) {
        part_impl(sv, "part2");
    }
}

int main() {
    Day5::part1("Input1.txt");
    Day5::part2("Input1.txt");
}