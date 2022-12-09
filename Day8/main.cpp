#include <iostream>
#include <string_View>
#include <fstream>
#include <string>
#include <vector>

namespace Day8 {
    struct Pair {
        int x, y;

        Pair(int x, int y) : x(x), y(y) {}
    };

    static bool is_visible_from_top(const Pair &pair, const std::vector<std::vector<int>> &tree_map, int value) {
        int max = 0;
        for (int i = 0; i < pair.y; i++) {
            max = std::max(max, tree_map.at(i).at(pair.x));
        }
        if (max < value) {
            return true;
        }
        return false;
    }

    static bool is_visible_from_bottom(const Pair &pair, const std::vector<std::vector<int>> &tree_map, int value) {
        int max = 0;
        for (int i = pair.y + 1; i < tree_map.size(); i++) {
            max = std::max(max, tree_map.at(i).at(pair.x));
        }
        if (max < value) {
            return true;
        }
        return false;
    }

    static bool is_visible_from_left(const Pair &pair, const std::vector<std::vector<int>> &tree_map, int value) {
        int max = 0;
        auto &vec = tree_map.at(pair.y);
        for (int i = 0; i < pair.x; i++) {
            max = std::max(max, vec.at(i));
        }
        if (max < value) {
            return true;
        }
        return false;
    }

    static bool is_visible_from_right(const Pair &pair, const std::vector<std::vector<int>> &tree_map, int value) {
        int max = 0;
        auto &vec = tree_map.at(pair.y);
        for (int i = pair.x + 1; i < vec.size(); i++) {
            max = std::max(max, vec.at(i));
        }
        if (max < value) {
            return true;
        }
        return false;
    }

    static bool is_visible(const Pair &pair, const std::vector<std::vector<int>> &tree_map) {
        int value = tree_map.at(pair.y).at(pair.x);
        return is_visible_from_top(pair, tree_map, value) ||
               is_visible_from_bottom(pair, tree_map, value) ||
               is_visible_from_left(pair, tree_map, value) ||
               is_visible_from_right(pair, tree_map, value);
    }

    static size_t calculate_left_score(const Pair &pair, const std::vector<std::vector<int>> &tree_map, int value) {
        int jumps = 0;
        auto &vec = tree_map.at(pair.y);
        for (int i = pair.x - 1; i >= 0; i--) {
            if (vec.at(i) < value) {
                jumps++;
            }
            if (vec.at(i) >= value) {
                jumps++;
                break;
            }
        }
        return jumps;
    }

    static size_t calculate_right_score(const Pair &pair, const std::vector<std::vector<int>> &tree_map, int value) {
        int jumps = 0;
        auto &vec = tree_map.at(pair.y);
        for (int i = pair.x + 1; i < vec.size(); i++) {
            if (vec.at(i) < value) {
                jumps++;
            }
            if (vec.at(i) >= value) {
                jumps++;
                break;
            }
        }
        return jumps;
    }

    static size_t calculate_top_score(const Pair &pair, const std::vector<std::vector<int>> &tree_map, int value) {
        int jumps = 0;
        for (int i = pair.y - 1; i >= 0; i--) {
            if (tree_map.at(i).at(pair.x) < value) {
                jumps++;
            }
            if (tree_map.at(i).at(pair.x) >= value) {
                jumps++;
                break;
            }
        }
        return jumps;
    }

    static size_t calculate_bottom_score(const Pair &pair, const std::vector<std::vector<int>> &tree_map, int value) {
        int jumps = 0;
        for (int i = pair.y + 1; i < tree_map.size(); i++) {
            if (tree_map.at(i).at(pair.x) < value) {
                jumps++;
            }
            if (tree_map.at(i).at(pair.x) >= value) {
                jumps++;
                break;
            }
        }
        return jumps;
    }

    static size_t calculate_scenic_score(const Pair &pair, const std::vector<std::vector<int>> &tree_map) {
        int value = tree_map.at(pair.y).at(pair.x);
        return calculate_top_score(pair, tree_map, value) *
               calculate_bottom_score(pair, tree_map, value) *
               calculate_left_score(pair, tree_map, value) *
               calculate_right_score(pair, tree_map, value);
    }

    static std::vector<std::vector<int>> generate_tree_map(std::string_view sv) {
        std::ifstream file(sv.data());

        std::vector<std::vector<int>> tree_map;
        for (std::string line; std::getline(file, line);) {
            std::vector<int> tmp;
            for (auto &c: line) {
                tmp.emplace_back(c - '0');
            }
            tree_map.emplace_back(tmp);
        }
        return tree_map;
    }


    static void part1(std::string_view sv) {
        auto tree_map = generate_tree_map(sv);

        size_t result = 0;
        for (int y = 0; y < tree_map.size(); y++) {
            auto &row = tree_map.at(y);
            if (y == 0 || y == tree_map.size() - 1) {
                // Top and bottom rows are always on the edge
                result += row.size();
                continue;
            }
            for (int x = 0; x < row.size(); x++) {
                if (x == 0 || x == row.size() - 1) {
                    // Edges of the row (1st and last)
                    result++;
                    continue;
                }
                if (is_visible(Pair(x, y), tree_map)) {
                    result++;
                }
            }
        }
        printf("Part1: %zu\n", result);
    }

    static void part2(std::string_view sv) {
        auto tree_map = generate_tree_map(sv);

        size_t max = 0;
        for (int y = 0; y < tree_map.size(); y++) {
            auto &row = tree_map.at(y);
            if (y == 0 || y == tree_map.size() - 1) {
                // Top and bottom rows are always on the edge
                continue;
            }
            for (int x = 0; x < row.size(); x++) {
                if (x == 0 || x == row.size() - 1) {
                    // Edges of the row (1st and last)
                    continue;
                }
                max = std::max(max, calculate_scenic_score(Pair(x, y), tree_map));
            }
        }
        printf("Part2: %zu\n", max);
    }
}

int main() {
    Day8::part1("Input1.txt");
    Day8::part2("Input1.txt");
}
