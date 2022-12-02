#include <iostream>
#include <fstream>
#include <string_view>

namespace Day2 {
    enum class Move {
        Rock = 1,
        Paper = 2,
        Scissors = 3
    };

    enum class GameResult {
        Loss = 0,
        Draw = 3,
        Win = 6
    };

    struct Pair {
        char a;
        char b;

        Move opponents_move() const {
            switch (a) {
                case 'A':
                    return Move::Rock;
                case 'B':
                    return Move::Paper;
                case 'C':
                    return Move::Scissors;
                default:
                    std::cout << "No such move" << std::endl;
            }
        }

        Move elves_move() const {
            switch (b) {
                case 'X':
                    return Move::Rock;
                case 'Y':
                    return Move::Paper;
                case 'Z':
                    return Move::Scissors;
                default:
                    std::cout << "No such move" << std::endl;
            }
        }

        // Pt2: How the game needs to ends.
        GameResult ends() const {
            switch (b) {
                case 'X':
                    return GameResult::Loss;
                case 'Y':
                    return GameResult::Draw;
                case 'Z':
                    return GameResult::Win;
                default:
                    std::cout << "No such move" << std::endl;
            }
        }
    };

    static int play_round_part1(const Pair &p) {
        GameResult result;
        if (p.opponents_move() == Move::Rock && p.elves_move() == Move::Paper) {
            result = GameResult::Win;
        } else if (p.opponents_move() == Move::Rock && p.elves_move() == Move::Scissors) {
            result = GameResult::Loss;
        } else if (p.opponents_move() == Move::Paper && p.elves_move() == Move::Scissors) {
            result = GameResult::Win;
        } else if (p.opponents_move() == Move::Paper && p.elves_move() == Move::Rock) {
            result = GameResult::Loss;
        } else if (p.opponents_move() == Move::Scissors && p.elves_move() == Move::Rock) {
            result = GameResult::Win;
        } else if (p.opponents_move() == Move::Scissors && p.elves_move() == Move::Paper) {
            result = GameResult::Loss;
        } else {
            result = GameResult::Draw;
        }

        return static_cast<int>(result) + static_cast<int>(p.elves_move());
    }

    static int play_round_part2(const Pair& p) {
        Move correct_move;

        if (p.ends() == GameResult::Draw) {
            if (p.opponents_move() == Move::Rock) {
                correct_move = Move::Rock;
            } else if (p.opponents_move() == Move::Paper) {
                correct_move = Move::Paper;
            } else if (p.opponents_move() == Move::Scissors) {
                correct_move = Move::Scissors;
            }
        } else if (p.ends() == GameResult::Win) {
            if (p.opponents_move() == Move::Rock) {
                correct_move = Move::Paper;
            } else if (p.opponents_move() == Move::Paper) {
                correct_move = Move::Scissors;
            } else if (p.opponents_move() == Move::Scissors) {
                correct_move = Move::Rock;
            }
        } else if (p.ends() == GameResult::Loss) {
            if (p.opponents_move() == Move::Rock) {
                correct_move = Move::Scissors;
            } else if (p.opponents_move() == Move::Paper) {
                correct_move = Move::Rock;
            } else if (p.opponents_move() == Move::Scissors) {
                correct_move = Move::Paper;
            }
        }

        return static_cast<int>(p.ends()) + static_cast<int>(correct_move);
    }

    static void part1(std::string_view sv) {
        std::ifstream file(sv.data());
        int result = 0;
        while (!file.eof()) {
            Pair p = {};
            file >> p.a >> p.b;
            if (p.a == '\0' || p.b == '\0') {
                break;
            }
            result += play_round_part1(p);
        }
        printf("Part1: %d\n", result);
    }

    static void part2(std::string_view sv) {
        std::ifstream file(sv.data());
        int result = 0;
        while (!file.eof()) {
            Pair p = {};
            file >> p.a >> p.b;
            if (p.a == '\0' || p.b == '\0') {
                break;
            }
            result += play_round_part2(p);
        }
        printf("Part2: %d\n", result);
    }
}

int main() {
    Day2::part1("Input1.txt");
    Day2::part2("Input1.txt");
}
