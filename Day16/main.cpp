#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>


struct Room {
    std::string name;
    std::vector <std::string> tunnels;
    int64_t pressure;

    // highest pressure first
//    auto operator<=>(const Room &other) {
//        if (other.pressure == pressure) {
//            return name <=> other.name;
//        }
//        return other.pressure <=> pressure;
//    }
};

std::unordered_map <std::string, Room> cave{};
std::map <std::string, Room> pressurized{};

// initialized by fillFloydWarshall()
std::map <std::pair<std::string, std::string>, int64_t> shortestPath{};

void fillFloydWarshall() {
    // https://en.wikipedia.org/wiki/Floyd%E2%80%93Warshall_algorithm
    for (const auto &[u, room]: cave) {
        for (const auto &v: room.tunnels) {
            shortestPath[{u, v}] = 1;
        }
        shortestPath[{u, u}] = 0;
    }
    for (const auto &[k, _1]: cave) {
        for (const auto &[i, _2]: cave) {
            if (shortestPath.find({i, k}) != shortestPath.end()) {
                continue;
            }
            for (const auto &[j, _3]: cave) {
                if (shortestPath.find({k, j}) != shortestPath.end()) {
                    continue;
                }

                if (shortestPath.find({i, j}) != shortestPath.end()) {
                    shortestPath[{i, j}] =
                            std::min(shortestPath[{i, j}],
                                     shortestPath[{i, k}] + shortestPath[{k, j}]);
                } else {
                    shortestPath[{i, j}] =
                            shortestPath[{i, k}] + shortestPath[{k, j}];
                }
            }
        }
    }
    printf("Populated %d paths\n", shortestPath.size());
}

std::unordered_map <std::string, int64_t> memo{};

int64_t findPathWithElephant(const int64_t minutes, const std::string &meRoom,
                             const std::string &eleRoom, const int64_t meTime,
                             const int64_t eleTime,
                             std::set <std::string> &valvesOpened) {
    if (valvesOpened.size() == pressurized.size()) {
        return 0;
    }

    std::stringstream memoKey1{};
    std::stringstream memoKey2{};
    for (const auto &valve: valvesOpened) {
        memoKey1 << valve;
    }
    memoKey2 << memoKey1.str();
    memoKey1 << meRoom << meTime << eleRoom << eleTime;
    memoKey2 << eleRoom << eleTime << meRoom << meTime;
    if (memo.find(memoKey1.str()) != memo.end()) {
        return memo[memoKey1.str()];
    }
    if (memo.find(memoKey2.str()) != memo.end()) {
        return memo[memoKey2.str()];
    }
    // fmt::print("memoKey1 = {}\n",memoKey1.str());
    // fmt::print("memoKey2 = {}\n",memoKey2.str());

    int64_t pressure = 0;
    for (const auto &[destName, destRoom]: pressurized) {
        if (valvesOpened.find(destName) != valvesOpened.end()) {
            continue;
        }
        const int64_t meReleaseTime =
                meTime + shortestPath[{meRoom, destName}] + 1;
        const int64_t eleReleaseTime =
                eleTime + shortestPath[{eleRoom, destName}] + 1;
        if (meReleaseTime <= eleReleaseTime) {
            // try me
            if (meReleaseTime <= minutes) {
                const int64_t releasePressure =
                        destRoom.pressure * (minutes - meReleaseTime);
                valvesOpened.insert(destName);
                const auto mePressure =
                        findPathWithElephant(minutes, destName, eleRoom,
                                             meReleaseTime, eleTime, valvesOpened);
                valvesOpened.erase(destName);

                pressure = std::max(pressure, releasePressure + mePressure);
            }
        }
        if (eleReleaseTime <= meReleaseTime) {
            // try my elephant
            if (eleReleaseTime <= minutes) {
                const int64_t releasePressure =
                        destRoom.pressure * (minutes - eleReleaseTime);
                valvesOpened.insert(destName);
                const auto elePressure =
                        findPathWithElephant(minutes, meRoom, destName, meTime,
                                             eleReleaseTime, valvesOpened);
                valvesOpened.erase(destName);

                pressure = std::max(pressure, releasePressure + elePressure);
            }
        }
    }

    memo[memoKey1.str()] = pressure;
    memo[memoKey2.str()] = pressure;
    return pressure;
}

int64_t findPathWithElephant(const int64_t minutes, const std::string &meRoom,
                             const std::string &eleRoom) {
    std::set <std::string> valvesOpened{};
    return findPathWithElephant(minutes, meRoom, eleRoom, 0, 0, valvesOpened);
}

int64_t findPath(const int64_t minutes, const std::string &roomName,
                 const int64_t time = 0,
                 std::set <std::string> valvesOpened = {}) {
    if (valvesOpened.size() == pressurized.size()) {
        return 0;
    }
    int64_t pressure = 0;
    for (const auto &[destName, destRoom]: pressurized) {
        if (valvesOpened.find(destName) != valvesOpened.end()) {
            continue;
        }
        const int64_t releaseTime =
                time + shortestPath[{roomName, destName}] + 1;
        if (releaseTime <= minutes) {
            const int64_t releasePressure =
                    destRoom.pressure * (minutes - releaseTime);
            valvesOpened.insert(destName);
            const auto subPressure =
                    findPath(minutes, destName, releaseTime, valvesOpened);
            valvesOpened.erase(destName);

            pressure = std::max(pressure, releasePressure + subPressure);
        }
    }

    return pressure;
}

int main(int argc, char **argv) {
    std::ifstream file("Example.txt");

    for (std::string line; std::getline(file, line);) {
        std::string valve = "Valve ";
        auto name = line.substr(valve.size(), 2);
        std::string rate_str = "rate=";
        auto rate = line.substr(line.find(rate_str) + rate_str.size());
        for (int i = 0; i < rate.size(); i++) {
            if (rate[i] == ';') {
                rate = rate.substr(0, i);
                break;
            }
        }
        std::string valves_str = "valves ";
        auto valves = line.substr(line.find(valves_str) + valves_str.size());
        std::stringstream ss(valves);
        decltype(Room::tunnels) dst{};
        while (!ss.eof()) {
            std::string word;
            std::getline(ss, word, ',');
            dst.push_back(word);
        }
        cave.emplace(name, Room{name, dst, std::stol(rate)});
        if (std::stol(rate) > 0) {
            pressurized.emplace(name, cave[name]);
        }
    }
    fillFloydWarshall();

    const auto releasedPressure = findPath(30, "AA");
    printf("You can release a pressure of %lld\n", releasedPressure);

    const auto releasedTwo = findPathWithElephant(26, "AA", "AA");
    printf("You and your elephant can release a pressure of %lld\n",
               releasedTwo);
}