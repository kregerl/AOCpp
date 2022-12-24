#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace Day24 {

    std::pair<int, int> add(std::pair<int, int> a, std::pair<int, int> b) { return {a.first + b.first, a.second + b.second}; }

    std::vector<char>& at(std::vector<std::vector<std::vector<char>>>& g, std::pair<int, int> p) { return g[p.first][p.second]; }

    std::vector<std::vector<std::vector<char>>> parse(std::istream&& is) {
        std::vector<std::vector<std::vector<char>>> g;
        std::string line;
        for (int row = 0; std::getline(is, line); row++) {
            g.emplace_back();
            for (int col = 0; col < line.size(); col++) {
                g.back().emplace_back();
                if (line[col] != '.') g.back().back().push_back(line[col]);
            }
        }
        return g;
    }

    std::pair<std::pair<int, int>, std::pair<int, int>> find_entrance_exit(const std::vector<std::vector<std::vector<char>>>& g) {
        auto find_empty = [](const auto& c) {
            auto it = std::find_if(c.begin(), c.end(), [](const auto& at_tile) {
                return at_tile.empty();
            });
            if (it == c.end()) {
                printf("no empty tile found\n");
                exit(0);
            }
            return it;
        };
        size_t entrance_col = find_empty(g.front()) - g.front().begin();
        size_t exit_col = find_empty(g.back()) - g.back().begin();
        return {{0,            entrance_col},
                {g.size() - 1, exit_col}};
    }

    enum dir {
        kDirsUp = 0, kDirsDown = 1, kDirsLeft = 2, kDirsRight = 3
    };
    constexpr std::pair<int, int> kDirs[] = {{-1, 0},
                            {1,  0},
                            {0,  -1},
                            {0,  1},
                            {0,  0}};

    std::pair<int, int> dir_for(char x) {
        switch (x) {
            case '<': return kDirs[kDirsLeft];
            case '^': return kDirs[kDirsUp];
            case '>': return kDirs[kDirsRight];
            case 'v': return kDirs[kDirsDown];
        }
        printf("bad dir\n");
        exit(0);
    }

    std::pair<int, int> wrap(const std::vector<std::vector<std::vector<char>>>& g, std::pair<int, int> p) {
        if (p.first == 0) p.first = g.size() - 2;
        if (p.first == g.size() - 1) p.first = 1;
        if (p.second == 0) p.second = g.back().size() - 2;
        if (p.second == g.back().size() - 1) p.second = 1;
        return p;
    }

    void step(std::vector<std::vector<std::vector<char>>>& g, std::vector<std::vector<std::vector<char>>>& scratch) {
        for (int row = 0; row < g.size(); row++) {
            for (int col = 0; col < g[row].size(); col++) {
                auto& cur = at(scratch, {row, col});
                if (cur.empty() || (cur.size() == 1 && cur[0] == '#')) continue;
                cur.clear();
            }
        }
        for (int row = 0; row < g.size(); row++) {
            for (int col = 0; col < g[row].size(); col++) {
                std::pair<int, int> p{row, col};
                for (auto storm: at(g, p)) {
                    if (storm == '#') break;
                    std::pair<int, int> q = wrap(g, add(p, dir_for(storm)));
                    at(scratch, q).push_back(storm);
                }
            }
        }
        g = scratch;
    }

    bool in_bounds(const std::vector<std::vector<std::vector<char>>>& g, std::pair<int, int> p) {
        return p.first >= 0 && p.first < g.size() && p.second >= 0 &&
               p.second < g[p.first].size();
    }

    int shortest_path(std::vector<std::vector<std::vector<std::vector<char>>>>& grids, std::pair<int, int> src, std::pair<int, int> dst, int dist) {
        std::set<std::pair<std::pair<int, int>, int>> v;
        v.emplace(src, dist);
        std::deque<std::pair<std::pair<int, int>, int>> q;
        q.emplace_back(src, dist);
        while (!q.empty()) {
            auto[cur, d] = q.front();
            auto& g = grids[d % grids.size()];
            auto& g_next = grids[(d + 1) % grids.size()];
            q.pop_front();
            for (int i = 0; i < 5; i++) {
                std::pair<int, int> nbr = add(cur, kDirs[i]);
                if (!in_bounds(g, nbr) || !at(g_next, nbr).empty()) continue;
                if (nbr == dst) return d + 1;
                int cycle = (d + 1) % grids.size();
                if (v.count({nbr, cycle})) continue;
                v.insert({nbr, cycle});
                q.emplace_back(nbr, d + 1);
            }
        }
        printf("No path\n");
        exit(0);
    }

    void print(const std::vector<std::vector<std::vector<char>>>& g, std::ostream& os = std::cout) {
        for (int row = 0; row < g.size(); row++) {
            for (int col = 0; col < g[row].size(); col++) {
                const auto& at_tile = g[row][col];
                if (at_tile.size() == 0) os << '.';
                else if (at_tile.size() == 1) os << at_tile[0];
                else os << at_tile.size();
            }
            os << std::endl;
        }
    }

    std::string k(const std::vector<std::vector<std::vector<char>>>& g) {
        std::stringstream s;
        print(g, s);
        return s.str();
    }

    std::vector<std::vector<std::vector<std::vector<char>>>> cycles(std::vector<std::vector<std::vector<char>>> g) {
        std::vector<std::vector<std::vector<std::vector<char>>>> cycles;
        cycles.push_back(g);
        std::vector<std::vector<std::vector<char>>> scratch = g;
        std::set<std::string> v;
        v.insert(k(g));
        for (int i = 0;; i++) {
            step(g, scratch);
            if (auto key = k(g); v.count(key)) break;
            else v.insert(key);
            cycles.push_back(g);
        }
        return cycles;
    }

    static void part1(std::string_view sv) {
        auto g = parse(std::ifstream(sv.data()));
        auto[src, dst] = find_entrance_exit(g);
        auto grids = cycles(g);
        int path1 = shortest_path(grids, src, dst, 0);
        std::cout << path1 << std::endl;
    }

    static void part2(std::string_view sv) {
        auto g = parse(std::ifstream(sv.data()));
        auto[src, dst] = find_entrance_exit(g);
        auto grids = cycles(g);
        int path1 = shortest_path(grids, src, dst, 0);
        int path2 = shortest_path(grids, dst, src, path1);
        int path3 = shortest_path(grids, src, dst, path2);
        std::cout << path3 << std::endl;
    }
}

int main() {
    Day24::part1("Input1.txt");
    Day24::part2("Input1.txt");
}