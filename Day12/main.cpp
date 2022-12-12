#include <iostream>
#include <string_view>
#include <fstream>
#include <vector>
#include <utility>

namespace Day12 {
    static int find_path(const std::vector<std::vector<int>>& map, std::vector<std::vector<int>>& min_path, const std::pair<int, int> end_pos, int goal) {
        int path_to_current = min_path[end_pos.first][end_pos.second] + 1;

        int lowest = INT32_MAX;
        for(int direction = 1; direction <= 7; direction+=2) {
            int nx = end_pos.first + ((direction%3) - 1);
            int ny = end_pos.second + ((direction/3) - 1);

            if(nx >= 0 && nx < map.size() && ny >= 0 && ny < map.at(0).size()) {
                bool can_step = map[nx][ny] <= map[end_pos.first][end_pos.second] + 1;
                bool is_shorter_path = (min_path[nx][ny] == 0 || path_to_current < min_path[nx][ny]);
                if(can_step && is_shorter_path) {
                    min_path[nx][ny] = path_to_current;
                    if(map[nx][ny] == goal) {
                        return path_to_current;
                    }
                    lowest = std::min(lowest, find_path(map, min_path, {nx, ny}, goal));
                }
            }
        }
        return lowest;
    }

    static std::vector<std::vector<int>> read_input(std::string_view sv) {
        std::ifstream file(sv.data());

        std::vector<std::vector<int>> map;
        for (std::string line; std::getline(file, line);) {
            std::vector<int> tmp;
            for (auto& c: line) {
                if (c == 'S') {
                    tmp.emplace_back(27);
                } else if (c == 'E') {
                    tmp.emplace_back(0);
                } else {
                    tmp.emplace_back('z' + 1 - c);
                }
            }
            map.emplace_back(tmp);
        }
        return map;
    }

    static void part1(std::string_view sv) {
        std::vector<std::vector<int>> map = std::move(read_input(sv));
        std::pair<int, int> end_pos;
        for (int x = 0; x < map.size(); x++) {
            for (int y = 0; y < map[x].size(); y++) {
                if (map[x][y] == 0) {
                    end_pos.first = x;
                    end_pos.second = y;
                }
            }
        }
        std::vector<std::vector<int>> min_known_path(map.size(), std::vector<int>(map.at(0).size()));
        auto result = find_path(map, min_known_path, end_pos, 27);
        printf("Part1: %d\n", result);
    }

    static void part2(std::string_view sv) {
        std::vector<std::vector<int>> map = std::move(read_input(sv));
        std::pair<int, int> end_pos;
        for (int x = 0; x < map.size(); x++) {
            for (int y = 0; y < map[x].size(); y++) {
                if (map[x][y] == 0) {
                    end_pos.first = x;
                    end_pos.second = y;
                }
            }
        }
        std::vector<std::vector<int>> min_known_path(map.size(), std::vector<int>(map.at(0).size()));
        auto result = find_path(map, min_known_path, end_pos, 26);
        printf("Part2: %d\n", result);
    }
}

int main() {
    Day12::part1("Input1.txt");
    Day12::part2("Input1.txt");
}
