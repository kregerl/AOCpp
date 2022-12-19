#include <algorithm>
#include <fstream>
#include <future>
#include <iostream>
#include <limits>
#include <array>
#include <string>
#include <vector>
#include "SimpleParser.h"

namespace Day19 {

    enum Material {
        ore = 0, clay = 1, obsidian = 2, geode = 3, none = -1
    };

    struct Blueprint {
        int64_t id{};
        std::array<int64_t, 6> costs{};
        std::array<int64_t, 4> maxBot{};

        Blueprint(const int id, const std::array<int64_t, 6> &costs)
                : id(id), costs(costs) {
            maxBot[ore] = std::max({costs[0], costs[1], costs[2], costs[4]});
            maxBot[clay] = costs[3];
            maxBot[obsidian] = costs[5];
            maxBot[geode] = std::numeric_limits<int64_t>::max();
        }

        std::array<int64_t, 3> buildCost(Material robot) const {
            switch (robot) {
                case ore:
                    return {costs[0], 0, 0};
                case clay:
                    return {costs[1], 0, 0};
                case obsidian:
                    return {costs[2], costs[3], 0};
                case geode:
                    return {costs[4], 0, costs[5]};
                default:
                    return {0, 0, 0};
            }
        }

        friend std::ostream &operator<<(std::ostream &out, const Blueprint &print) {
            return out << "Blueprint " << print.id << ": \n";
        }
    };

    struct World {
        std::array<int64_t, 4> robots{1, 0, 0, 0};
        std::array<int64_t, 4> materials{0, 0, 0, 0};

        bool canBuild(const Blueprint &print, const Material bot) const {
            if (bot == none) {
                return true;
            }
            if (robots[bot] >= print.maxBot[bot]) {
                // don't build more bots than needed
                return false;
            }
            const auto cost = print.buildCost(bot);
            for (const auto m: {ore, clay, obsidian}) {
                if (materials[m] < cost[m]) {
                    return false;
                }
            }
            return true;
        }

        void startBuild(const Blueprint &print, const Material bot) {
            if (bot == none) {
                return;
            }
            const auto cost = print.buildCost(bot);
            for (const auto m: {ore, clay, obsidian}) {
                materials[m] -= cost[m];
            }
        }

        bool supplyCompleted(const Blueprint &print) const {
            return robots[ore] == print.maxBot[ore] and
                   robots[clay] == print.maxBot[clay] and
                   robots[obsidian] == print.maxBot[obsidian] and
                   canBuild(print, geode);
        }

    };

    std::vector<Blueprint> blueprints;

    int64_t geodeAmount(const Blueprint &print, int64_t minutesLeft, World world,
                        const Material buildingBot, int64_t &geodesMonitor) {

        if (buildingBot != none) {
            world.startBuild(print, buildingBot);
        }
        for (const auto mat: {ore, clay, obsidian, geode}) {
            world.materials[mat] += world.robots[mat];
        }
        if (buildingBot != none) {
            ++world.robots[buildingBot];
        }
        --minutesLeft;

        if (minutesLeft == 0) {
            if (geodesMonitor < world.materials[geode]) {
                geodesMonitor = world.materials[geode];
            }
            return world.materials[geode];
        } else if (minutesLeft == 1) {
            return geodeAmount(print, 1, world, none, geodesMonitor);
        }

        const auto canHarvestGeodes = world.materials[geode] +
                                      world.robots[geode] * minutesLeft +
                                      (minutesLeft - 1) * minutesLeft / 2;
        if (canHarvestGeodes <= geodesMonitor) {
            return 0;
        }
        if (world.supplyCompleted(print)) {
            geodesMonitor = canHarvestGeodes;
            return canHarvestGeodes;
        }

        int64_t maxGeodes = 0;
        for (const Material bot: {geode, obsidian, clay, ore, none}) {
            if (world.canBuild(print, bot)) {
                maxGeodes =
                        std::max(maxGeodes, geodeAmount(print, minutesLeft, world, bot,
                                                        geodesMonitor));
            }
        }
        return maxGeodes;
    }

    int64_t geodeAmount(const Blueprint &print, int64_t minutes) {
        int64_t geodesMonitor = 0;
        // turn 1 does not require thinking, we can't build anything yet
        return geodeAmount(print, minutes, {}, none, geodesMonitor);
    }

    int64_t qualityLevel(const Blueprint &print) {
        return geodeAmount(print, 24) * print.id;
    }

    static void solve(std::string_view sv) {
            std::ifstream file(sv.data());
            SimpleParser scanner{file};
            while (!scanner.isEof()) {
                auto v = scanner.skipToken("Blueprint");
                const auto id = scanner.getInt64();
                v |= scanner.skipToken(": Each ore robot costs");
                const auto c0 = scanner.getInt64();
                v |= scanner.skipToken("ore. Each clay robot costs");
                const auto c1 = scanner.getInt64();
                v |= scanner.skipToken("ore. Each obsidian robot costs");
                const auto c2 = scanner.getInt64();
                v |= scanner.skipToken("ore and");
                const auto c3 = scanner.getInt64();
                v |= scanner.skipToken("clay. Each geode robot costs");
                const auto c4 = scanner.getInt64();
                v |= scanner.skipToken("ore and");
                const auto c5 = scanner.getInt64();
                v |= scanner.skipToken("obsidian.");
                if (v) {
                    blueprints.emplace_back(
                            id, std::array<int64_t, 6>{c0, c1, c2, c3, c4, c5});
                } else {
                    printf("Error reading blueprint id: %lld\n", id);
                }
            }

            std::vector<std::future<int64_t>> taskpool1{};
            std::vector<std::future<int64_t>> taskpool2{};

            for (int i = 0; i < 3; i++) {
                if (i < (int64_t) blueprints.size()) {
                    taskpool2.push_back(std::async(
                            std::launch::async,
                            [](const auto num) { return geodeAmount(blueprints[num], 32); },
                            i));
                }
            }
            for (const auto &blueprint: blueprints) {
                taskpool1.push_back(
                        std::async(std::launch::async, qualityLevel, blueprint));
            }

            int64_t sum = 0;
            int64_t id = 0;
//            for (auto &result: taskpool1) {
//                printf("Blueprint %lld: ", ++id);
//                std::cout << std::flush;
//                const auto geodes = result.get();
//                printf("%lld geodes\n", geodes);
//                sum += geodes;
//            }
//            printf("The total quality level is %lld\n", sum);

            int64_t prod = 1;
            id = 0;
            for (auto &result: taskpool2) {
                printf("Blueprint %lld: ", ++id);
                std::cout << std::flush;
                const auto geodes = result.get();
                printf("%lld geodes\n", geodes);
                prod *= geodes;
            }
            printf("The product of the first 3 blueprints is %lld\n", prod);
    };
}

int main() {
    Day19::solve("Input1.txt");
}