#include <iostream>
#include <string_view>
#include <fstream>
#include <set>
#include <utility>

#define ROPE_SIZE 10

namespace Day9 {

    typedef std::pair<int, int> Pair;

    enum class MoveDirection {
        Up = 0,
        Down,
        Left,
        Right
    };

    struct Movement {
        MoveDirection direction;
        int amount;

        explicit Movement(const std::string& line) {
            size_t pos;
            if ((pos = line.find(' ')) != std::string::npos) {
                std::string direction_str = line.substr(0, pos);
                if (direction_str.size() != 1) {
                    printf("Direction size is too big: %s\n", direction_str.c_str());
                    return;
                }
                amount = std::stoi(line.substr(pos));
                char dir = direction_str.front();
                switch (dir) {
                    case 'U': {
                        direction = MoveDirection::Up;
                        break;
                    }
                    case 'D': {
                        direction = MoveDirection::Down;
                        break;
                    }
                    case 'L': {
                        direction = MoveDirection::Left;
                        break;
                    }
                    case 'R': {
                        direction = MoveDirection::Right;
                        break;
                    }
                    default: {
                        printf("Unknown direction: %c\n", dir);
                    }
                }
            }

        }
    };

    static bool is_adjacent(const Pair& head, const Pair& tail) {
        for (int i = tail.first - 1; i <= tail.first + 1; i++) {
            for (int j = tail.second - 1; j <= tail.second + 1; j++) {
                // I and J are not negative and head is adjacent to tail.
                if (i == head.first && j == head.second) {
                    return true;
                }
            }
        }

        return false;
    }

    static void part1(std::string_view sv) {
        std::ifstream file(sv.data());

        Pair head(0, 0);
        Pair tail = head;
        std::set<Pair> positions;
        positions.insert(tail);

        for (std::string line; std::getline(file, line);) {
            Movement move(line);
            // 1 Movement at a time
            for (int i = 0; i < move.amount; i++) {
                switch (move.direction) {
                    case MoveDirection::Up: {
                        head.second++;
                        if (!is_adjacent(head, tail)) {
                            tail.second = head.second - 1;
                            tail.first = head.first;
                            positions.insert(Pair(tail.first, tail.second));
                        }
                        break;
                    }
                    case MoveDirection::Down: {
                        head.second--;
                        if (!is_adjacent(head, tail)) {
                            tail.second = head.second + 1;
                            tail.first = head.first;
                            positions.insert(Pair(tail.first, tail.second));
                        }
                        break;
                    }
                    case MoveDirection::Left: {
                        head.first--;
                        if (!is_adjacent(head, tail)) {
                            tail.first = head.first + 1;
                            tail.second = head.second;
                            positions.insert(Pair(tail.first, tail.second));
                        }
                        break;
                    }
                    case MoveDirection::Right: {
                        head.first++;
                        if (!is_adjacent(head, tail)) {
                            tail.first = head.first - 1;
                            tail.second = head.second;
                            positions.insert(Pair(tail.first, tail.second));
                        }
                        break;
                    }
                }
            }
        }
        printf("Part1: %lu\n", positions.size());
    }

    int dx(const Pair& parent, const Pair& child) {
        if (parent.first == child.first) {
            return 0;
        }
        return parent.first > child.first ? 1 : -1;
    }

    int dy(const Pair& parent, const Pair& child) {
        if (parent.second == child.second) {
            return 0;
        }
        return parent.second > child.second ? 1 : -1;
    }

    static void part2(std::string_view sv) {
        std::ifstream file(sv.data());


        Pair rope[ROPE_SIZE];
        std::fill(std::begin(rope), std::end(rope), Pair(0, 0));

        std::set<Pair> positions;
        positions.insert(rope[9]);

        for (std::string line; std::getline(file, line);) {
            Movement move(line);
            // 1 Movement at a time
            for (int i = 0; i < move.amount; i++) {
                switch (move.direction) {
                    case MoveDirection::Up: {
                        rope[0].second += 1;
                        break;
                    }
                    case MoveDirection::Down:{
                        rope[0].second -= 1;
                        break;
                    }
                    case MoveDirection::Left: {
                        rope[0].first -= 1;
                        break;
                    }
                    case MoveDirection::Right: {
                        rope[0].first += 1;
                        break;
                    }
                }
                for (int j = 1; j < ROPE_SIZE; j++) {
                    Pair parent = rope[j-1];
                    Pair child = rope[j];
                    if (is_adjacent(parent, child)) {
                        break;
                    }

                    rope[j] = Pair(child.first + dx(parent, child), child.second + dy(parent, child));
                }
                positions.insert(rope[9]);
            }
        }
        printf("Part2: %lu\n", positions.size());
    }
}

int main() {
    Day9::part1("Input1.txt");
    Day9::part2("Input1.txt");
}
