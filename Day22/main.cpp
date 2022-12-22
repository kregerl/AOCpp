#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "SimpleParser.h

// -1: turn left, +1: turn right
enum facing { right = 0, down = 1, left = 2, up = 3 };
static const std::vector<Vec2l> direction{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

// strange map, with border intervals in Vec2l
std::unordered_map<Vec2l, char> stMap{};
std::vector<Vec2l> stCols{};
std::vector<Vec2l> stRows{};
int64_t maxCol{0}; // stCols[x] = {minY,maxY}
int64_t maxRow{0}; // stRows[y] = {minX,maxX}

Vec2l getNextPos(const Vec2l &myPos, const int64_t myFace) {
    auto nextPos = myPos + direction[myFace];
    switch (myFace) {
        case right:
            if (nextPos.x > stRows[myPos.y].y)
                nextPos.x = stRows[myPos.y].x;
            break;
        case left:
            if (nextPos.x < stRows[myPos.y].x)
                nextPos.x = stRows[myPos.y].y;
            break;
        case down:
            if (nextPos.y > stCols[myPos.x].y)
                nextPos.y = stCols[myPos.x].x;
            break;
        case up:
            if (nextPos.y < stCols[myPos.x].x)
                nextPos.y = stCols[myPos.x].y;
    }
    return nextPos;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input.txt>\n";
        std::exit(EXIT_FAILURE);
    }

    std::ifstream infile{argv[1]};
    std::string line;

    // int64_t row=1;
    for (int64_t row = 1; std::getline(infile, line); ++row) {
        // while (std::getline(infile, line)) {
        if (line.size() == 0)
            break;
        for (const auto i : iota(0u, line.size())) {
            if (line[i] != ' ') {
                const int64_t col = i + 1;
                stMap[{col, row}] = line[i];
                maxCol = std::max(maxCol, col);
            }
        }
        maxRow = row;
    }
    stCols.resize(maxCol + 1);
    stRows.resize(maxRow + 1);

    for (const auto y : iota(1, maxRow + 1)) {
        for (auto x = 1; x <= maxCol; ++x) {
            if (stMap.contains({x, y})) {
                const auto minX = x;
                while (stMap.contains({++x, y}))
                    ;
                const auto maxX = x - 1;
                stRows[y] = {minX, maxX};
                // fmt::print("[{}-{},{}]\n", minX,maxX, y);
                break;
            }
        }
    }

    for (const auto x : iota(1, maxCol + 1)) {
        for (auto y = 1; y <= maxRow; ++y) {
            if (stMap.contains({x, y})) {
                const auto minY = y;
                while (stMap.contains({x, ++y}))
                    ;
                const auto maxY = y - 1;
                stCols[x] = {minY, maxY};
                // fmt::print("[{},{}-{}]\n",x,minY,maxY);
                break;
            }
        }
    }

    // fmt::print("Map {}x{}\n",maxCol,maxRow);

    Vec2l myPos{stRows[1].x, 1};
    int64_t myFace = right;

    SimpleParser scanner{infile};
    while (!scanner.isEof()) {
        const auto len = scanner.getInt64();
        // fmt::print("forward {}\n",len);
        for ([[maybe_unused]] const auto i : iota(0, len)) {
            const auto nextPos = getNextPos(myPos, myFace);
            if (!stMap.contains(nextPos))
                fmt::print("error {}\n", nextPos);
            if (stMap[nextPos] == '#') {
                break;
            }
            myPos = nextPos;
        }
        if (scanner.isEof()) {
            break;
        }
        // TODO scanner api
        const auto turn = scanner.peekChar();
        scanner.skipChar(turn);
        // fmt::print("turn {}\n",turn);
        switch (turn) {
            case 'L':
                myFace = (myFace + 3) % 4;
                break;
            case 'R':
                myFace = (myFace + 1) % 4;
                break;
        }
    }

    // fmt::print("({}, {}) face {}\n", myPos.x, myPos.y, myFace);
    fmt::print("The password is {}\n", 1000 * myPos.y + 4 * myPos.x + myFace);
}