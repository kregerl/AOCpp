#include <iostream>
#include <string_view>
#include <fstream>
#include <stack>
#include <vector>
#include <sstream>

#define NUM_COLUMNS 40

namespace Day10 {

    struct Instruction {
        std::string instruction;
        int amount;

        Instruction(std::string instruction, int amount) : instruction(std::move(instruction)), amount(amount) {}

        [[nodiscard]] int num_cycles() const {
            return instruction == "noop" ? 1 : 2;
        }
    };

    struct State {
        int x;
        int cycle;

        explicit State(int x = 1, int cycle = 0) : x(x), cycle(cycle) {}
    };

    struct ExecRange {
        int x;
        int cycle_start;
        int cycle_end;

        [[nodiscard]] bool has_cycle(int cycle) const {
            return cycle >= cycle_start && cycle <= cycle_end;
        }
    };

    static ExecRange exec(const Instruction& instruction, State* current_state) {
        ExecRange range = {
                .x = current_state->x,
                .cycle_start = current_state->cycle + 1,
                .cycle_end = current_state->cycle + instruction.num_cycles()
        };

        if (instruction.instruction != "noop") {
            current_state->x += instruction.amount;
        }
        current_state->cycle = range.cycle_end;
        return range;
    }

    static std::vector<Instruction> read_instructions(std::string_view sv) {
        std::ifstream file(sv.data());

        std::vector<Instruction> instructions;
        for (std::string line; std::getline(file, line);) {
            size_t pos = line.find(' ');
            std::string instruction = line.substr(0, pos);
            std::string amt = line.substr(pos + 1);
            int amount = amt == instruction ? 0 : std::stoi(amt);
            instructions.emplace_back(Instruction(instruction, amount));
        }
        return instructions;
    }

    static void part1(std::string_view sv) {
        std::vector<Instruction> instructions = std::move(read_instructions(sv));
        State state;
        std::vector<int> signals;
        std::vector<int> important_cycles = {20, 60, 100, 140, 180, 220};
        auto current_important_cycle = important_cycles.begin();
        for (const auto& instruction: instructions) {
            ExecRange range = exec(instruction, &state);
            if (current_important_cycle != important_cycles.end() && range.has_cycle(*current_important_cycle)) {
                signals.emplace_back(*(current_important_cycle++) * range.x);
            }
        }

        int result = 0;
        for (auto& signal: signals) {
            result += signal;
        }
        printf("Part1: %d\n", result);
    }

    static void part2(std::string_view sv) {
        std::vector<Instruction> instructions = std::move(read_instructions(sv));
        State state;
        std::stringstream ss;
        for (const auto& instruction : instructions) {
            ExecRange range = exec(instruction, &state);
            for (int cycle = range.cycle_start; cycle <= range.cycle_end; ++cycle) {
                int pos = (cycle - 1) % NUM_COLUMNS;
                if (pos >= range.x - 1 && pos <= range.x + 1) {
                    ss << '#';
                } else {
                    ss << '.';
                }
                if (pos + 1 == NUM_COLUMNS) {
                    ss << "\n";
                }
            }
        }
        std::cout << "================ Part2 =================" << std::endl;
        std::cout << ss.str() << std::endl;
    }
}

int main() {
    Day10::part1("Input1.txt");
    // RKAZAJBR
    Day10::part2("Input1.txt");
}
