#include <iostream> 
#include <fstream>
#include "Chamber.h"

//namespace Day17 {
//
//
//    static void part1(std::string_view sv) {
//        std::ifstream file(sv.data());
//        std::string jets;
//        std::getline(file, jets);
//        Chamber c;
//        c.place_rock(Rock::Plus);
//    }
//
//}
//
//int main() {
//    Day17::part1("Example.txt");
//}
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>


struct Jets;
struct Rock;
struct Shape;
struct Tower;

struct Vec2l {
    long x, y;

    Vec2l operator+(const Vec2l& rhs) const {
        long nx = x + rhs.x;
        long ny = y + rhs.y;
        return {nx, ny};
    }

    Vec2l operator-(const Vec2l& rhs) const {
        long nx = x + rhs.x;
        long ny = y + rhs.y;
        return {nx, ny};
    }

    Vec2l& operator+=(const Vec2l& rhs) {
        x += rhs.x;
        return *this;
    }
};

struct Tower {
    std::vector<std::string> pile{};
    int64_t nextShape{0};
    std::array<int64_t, 7> topPos{0, 0, 0, 0, 0, 0, 0};

    Rock spawnRock();
    void addRock(const Rock &rock);
    int64_t height() const;
    bool isSolid(const Vec2l &pos) const;
    void print() const;
    void print(const Rock &current) const;
};

struct Shape {
    const std::vector<std::string> s;

    int64_t width() const { return s.at(0).size(); }

    int64_t height() const { return s.size(); }

    bool isSolid(const Vec2l &pos) const { return s.at(pos.x)[pos.y] == '#'; }
};

static const std::vector<Shape> shapes{{{{"####"}},
                                        {{".#.", "###", ".#."}},
                                        {{"###", "..#", "..#"}},
                                        {{"#", "#", "#", "#"}},
                                        {{"##", "##"}}}};

struct Jets {
    const std::string &pattern;
    size_t position{};
    Jets(std::string &in) : pattern(in), position(in.size()) {}
    int64_t direction() {
        if (++position >= pattern.size()) {
            position = 0;
        }
        switch (pattern[position]) {
            case '<':
                return -1;
            case '>':
                return 1;
            default:
                ++position;
                return direction();
        }
    }
};

struct Rock {
    Tower &tower;
    Vec2l pos{}; // bottom left
    int64_t shape;
    bool falling{};

    Rock(Tower &tower, const Vec2l &pos, int64_t shape)
            : tower(tower), pos(pos), shape(shape), falling(true) {}

    bool isFalling() const { return falling; }

    bool canMove(const Vec2l &dest) const {
        for (int x = 0; x < width(); x++) {
            for (int y = 0; y < height(); y++) {
                if (tower.isSolid(dest + Vec2l{x, y}) and
                    this->isSolid(pos + Vec2l{x, y})) {
                    return false;
                }
            }
        }
        return true;
    }

    void blow(int64_t dir) {
        if (canMove(pos + Vec2l{dir, 0})) {
            pos += Vec2l{dir, 0};
        }
    }

    void drop() {
        if (canMove(pos + Vec2l{0, -1})) {
            pos += Vec2l{0, -1};
        } else {
            falling = false;
            tower.addRock(*this);
        }
    }

    int64_t width() const { return shapes.at(shape).width(); }

    int64_t height() const { return shapes.at(shape).height(); }

    bool isSolid(const Vec2l &loc) const {
        const auto rPos = loc - pos;
        if (rPos.x < 0 or rPos.y < 0 or rPos.x >= width() or
            rPos.y >= height()) {
            return false;
        }
        return shapes.at(shape).isSolid(loc - pos);
    }
};

Rock Tower::spawnRock() {
    nextShape %= 5;
    return Rock(*this, {2, height() + 3}, nextShape++);
}

void Tower::addRock(const Rock &rock) {
    for (int y = rock.pos.y; y < rock.pos.y + rock.height(); y++) {
        for (int x  = rock.pos.x; x <  rock.pos.x + rock.width(); x++) {
            if (rock.isSolid({x, y})) {
                while (y >= (int64_t)pile.size()) {
                    pile.push_back(".......");
                }
                pile[y][x] = '#';
                topPos[x] = y;
            }
        }
    }
}

int64_t Tower::height() const { return pile.size(); }

bool Tower::isSolid(const Vec2l &pos) const {
    if (pos.y < 0) {
        return true;
    }
    if (pos.x < 0 or pos.x > 6) {
        return true;
    }
    if (pos.y < height()) {
        return pile[pos.y][pos.x] == '#';
    } else {
        return false;
    }
}

void Tower::print() const {
    const Rock fake{const_cast<Tower &>(*this), {-10, -10}, 0};
    print(fake);
}

void Tower::print(const Rock &current) const {
    const auto maxY = std::max(height(), current.pos.y + current.height());
    for (int y = -maxY; y < 1; y++) {
//    for (const auto y : iota(-maxY, 1) | std::views::transform([](auto x) { return -x; })) {
        std::cout << '|';
        for (int x = 0; x < 7; x++) {
            if (current.isSolid({x, y})) {
                if (current.isFalling()) {
                    std::cout << '@';
                } else {
                    std::cout << '=';
                }
            } else {
                if (this->isSolid({x, y})) {
                    std::cout << '#';
                } else {
                    std::cout << '.';
                }
            }
        }
        std::cout << "|\n";
    }
    std::cout << "+-------+\n";
    std::cout << "Size: " << height() << "\n\n";
}

int64_t level(std::string &input, int64_t rounds) {
    Tower tower{};
    Jets jets{input};
    int64_t fallenRocks = 0;
    int64_t wallClock = 0;
    int64_t addTowerHeight = 0;

    struct State {
        int64_t rocks;
        int64_t height;
        int64_t clock;
    };
    std::unordered_map<std::string, State> loopDetect{};

    while (fallenRocks < rounds) {
        auto rock = tower.spawnRock();
        while (rock.isFalling()) {
            rock.blow(jets.direction());
            rock.drop();
            ++wallClock;

            // check for repeats
            if (wallClock % input.size() == 0) {
                std::stringstream hash{};
                for (const auto n : tower.topPos) {
                    hash << tower.height() - n << ':';
                }
                hash << rock.pos.x << ':' << rock.pos.y - tower.height() << ':'
                     << rock.shape;
                if (!loopDetect.contains(hash.str())) {
                    loopDetect[hash.str()] =
                            State{fallenRocks, tower.height(), wallClock};
                } else {
                    // advance loop and game state here
                    const auto &prev = loopDetect[hash.str()];
                    State loopSize = {fallenRocks - prev.rocks,
                                      tower.height() - prev.height,
                                      wallClock - prev.clock};
                    printf("Loop detected over %ld rocks!\n", loopSize.rocks);
                    const int64_t loopsToGo =
                            (rounds - fallenRocks - 1) / loopSize.rocks;
                    // advance!
                    fallenRocks += loopSize.rocks * loopsToGo;
                    addTowerHeight = loopSize.height * loopsToGo;
                    wallClock += loopSize.clock * loopsToGo;
                    // There may be other loops of the same height, forget them
                    loopDetect.clear();
                }
            }
        }
        ++fallenRocks;
    }
    return tower.height() + addTowerHeight;
}

int64_t level1(std::string &input) { return level(input, 2022); }

int64_t level2(std::string &input) { return level(input, 1000000000000); }

int main(int argc, char **argv) {
    std::ifstream infile("Example.txt");
    std::string line;
    std::getline(infile, line);
    printf("The 2022 rocks tower is %ld tall\n", level1(line));
    printf("The 1000000000000 rocks tower is %ld tall\n", level2(line));
}