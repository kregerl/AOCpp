#include <iostream>
#include <vector>
#include <set>
#include <cstdio>
#include <climits>
#include <fstream>
#include <cmath>

#define row first
#define col second

namespace Day23 {
    std::pair<int, int> operator+(const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return std::make_pair(a.row + b.row, a.col + b.col);
    }

// Program state
    std::set<std::pair<int, int>> monkeys;
    std::pair<int, int> N = {-1, 0}, S = {1, 0}, W = {0, -1}, E = {0, 1};
    std::pair<int, int> NE = N + E, NW = N + W;
    std::pair<int, int> SE = S + E, SW = S + W;
    std::pair<int, int> dirs[] = {N, S, W, E};
    int start = 0;

    static void printMonkeys(FILE* fp) {
        int minRow = INT_MAX, minCol = INT_MAX;
        int maxRow = INT_MIN, maxCol = INT_MIN;
        for (std::pair<int, int> m: monkeys) {
            if (m.row < minRow) minRow = m.row;
            if (m.row > maxRow) maxRow = m.row;
            if (m.col < minCol) minCol = m.col;
            if (m.col > maxCol) maxCol = m.col;
        }

        for (int row = minRow; row <= maxRow; row++) {
            for (int col = minCol; col <= maxCol; col++) {
                std::pair<int, int> current = {row, col};
                char ch = monkeys.count(current) == 0 ? '.' : '#';
                fprintf(fp, "%c", ch);
            }
            fprintf(fp, "\n");
        }

        for (std::pair<int, int> m: monkeys) {
            if (m.row < minRow) minRow = m.row;
            if (m.row > maxRow) maxRow = m.row;
            if (m.col < minCol) minCol = m.col;
            if (m.col > maxCol) maxCol = m.col;
        }

        int rows = maxRow - minRow + 1;
        int cols = maxCol - minCol + 1;
        int total = rows * cols;
        int empty = total - monkeys.size();
        printf("Empty: %d\n", empty);
    }

    static std::pair<int, int> getPropose(std::pair<int, int> m) {
        // Look around to check stay case
        bool stay = true;
        for (int rowDiff = -1; stay && rowDiff <= 1; rowDiff++)
            for (int colDiff = -1; stay && colDiff <= 1; colDiff++) {
                if (rowDiff == 0 && colDiff == 0)
                    continue;

                // Found a neighbor
                std::pair<int, int> p = {m.row + rowDiff, m.col + colDiff};
                if (monkeys.count(p) > 0)
                    stay = false;
            }
        if (stay)
            return m;

        for (int i = 0; i < 4; i++) {
            std::pair<int, int> dir = dirs[(start + i) % 4];
            if (dir == N) {
                if (monkeys.count(m + N) == 0
                    && monkeys.count(m + NE) == 0
                    && monkeys.count(m + NW) == 0)
                    return m + N;
            } else if (dir == S) {
                if (monkeys.count(m + S) == 0
                    && monkeys.count(m + SE) == 0
                    && monkeys.count(m + SW) == 0)
                    return m + S;
            } else if (dir == W) {
                if (monkeys.count(m + W) == 0
                    && monkeys.count(m + NW) == 0
                    && monkeys.count(m + SW) == 0)
                    return m + W;
            } else if (dir == E) {
                if (monkeys.count(m + E) == 0
                    && monkeys.count(m + NE) == 0
                    && monkeys.count(m + SE) == 0)
                    return m + E;
            } else {
                printf("Invalid dir\n");
                exit(1);
            }
        }

        return m;

    }

    static bool round(int roundi) {
        // Store proposes
        std::vector<std::pair<int, int>> pVec;
        std::multiset<std::pair<int, int>> pSet;

        // Store order of monkeys
        std::vector<std::pair<int, int>> mVec;

        for (std::pair<int, int> m: monkeys) {
            mVec.push_back(m);

            std::pair<int, int> p = getPropose(m);
            pVec.push_back(p);
            pSet.insert(p);
        }

        int i = 0;
        bool moving = false;
        for (std::pair<int, int> m: mVec) {
            std::pair<int, int> p = pVec[i++];
            if (m != p) moving = true;

            if (m != p && pSet.count(p) == 1) {

                if (monkeys.count(p)) {
                    std::cout << std::endl << "Stepping: " << std::endl;
                    std::cerr << "You can't step on a monkey!" << std::endl;
                    exit(1);
                }

                monkeys.erase(m);
                monkeys.insert(p);
            }
        }

        start++;

        return moving;
    }

    static void round() {
        // Store proposes
        std::vector<std::pair<int, int>> pVec;
        std::multiset<std::pair<int, int>> pSet;

        // Store order of monkeys
        std::vector<std::pair<int, int>> mVec;

        for (std::pair<int, int> m: monkeys) {
            mVec.push_back(m);

            std::pair<int, int> p = getPropose(m);
            pVec.push_back(p);
            pSet.insert(p);
        }

        int i = 0;
        for (std::pair<int, int> m: mVec) {
            std::pair<int, int> p = pVec[i++];
            if (m != p && pSet.count(p) == 1) {

                if (monkeys.count(p)) {
                    std::cout << std::endl << "Stepping: " << std::endl;
                    std::cerr << "You can't step on a monkey!" << std::endl;
                    exit(1);
                }

                monkeys.erase(m);
                monkeys.insert(p);
            }
        }

        start++;
    }

    static void part1(std::string_view sv, int rounds) {
        std::ifstream file(sv.data());

        std::string line;
        for (int row = 1; getline(file, line); row++)
            for (int i = 0; i < line.size(); i++)
                if (line[i] == '#')
                    monkeys.insert({row, i + 1});

        FILE* parse = fopen("PARSE", "w");
        printMonkeys(parse);
        fclose(parse);

        FILE* state = fopen("STATE", "w");
        fprintf(state, "== Initial State ==\n");
        fprintf(stdout, "== Initial State ==\n");
        printMonkeys(state);
        fprintf(state, "\n");

        for (int i = 0; i < rounds; i++) {
            round();
            fprintf(state, "== End of Round %d ==\n", i + 1);
            fprintf(stdout, "== End of Round %d ==\n", i + 1);
            printMonkeys(state);
            fprintf(state, "\n");
            fprintf(stdout, "\n");
        }

        fclose(state);
    }

    static void part2(std::string_view sv, int rounds) {
        std::ifstream file(sv.data());

        std::string line;
        for (int row = 1; getline(file, line); row++)
            for (int i = 0; i < line.size(); i++)
                if (line[i] == '#')
                    monkeys.insert({row, i + 1});


        bool moving = false;
        for (int i = 0; i < rounds; i++) {
            moving = round(i + 1);

            if ((i + 1) % 100 == 0) {
                fprintf(stdout, "== End of Round %d ==\n", i + 1);
            }

            if (!moving) {
                std::cout << "Success" << std::endl;
                printf("%d\n", i + 1);
                break;
            }
        }

        if (moving)
            std::cout << "Failure" << std::endl;
    }
}

int main() {
    Day23::part1("Input1.txt", 10);
    Day23::part2("Input1.txt", INT_MAX);
}