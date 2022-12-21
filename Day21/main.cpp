#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <cstdio>
#include <fstream>

namespace Day21 {
    struct Action {
        std::string m1;
        char op;
        std::string m2;
    };

    struct Monkey {
        std::string name;
        bool isNumber;
        long n;
        Action act;
    };

    std::map<std::string, Monkey> monkeys;

    std::set<std::string> dependOnHumn;

    static Monkey parseMonkey(std::string line) {
        Monkey m;

        char nameC[4 + 1];
        char m1C[4 + 1];
        char op;
        char m2C[4 + 1];
        long case1 = sscanf(line.c_str(), "%4s: %s %c %s",
                            nameC, m1C, &op, m2C);

        if (case1 == 4) {
            std::string name(nameC);
            std::string m1(m1C);
            std::string m2(m2C);

            m = {
                    .name = name,
                    .isNumber = false,
                    .act = {m1, op, m2}
            };
        } else {
            long n;
            sscanf(line.c_str(), "%4s: %ld", nameC, &n);
            std::string name(nameC);
            m = {
                    .name = name,
                    .isNumber = true,
                    .n = n
            };
        }

        return m;
    }

    static long getVal(std::string name) {
        Monkey m = monkeys[name];
        if (m.isNumber)
            return m.n;

        Action act = m.act;
        long n1 = getVal(act.m1);
        long n2 = getVal(act.m2);

        long val;
        switch (act.op) {
            case '+':val = n1 + n2;
                break;
            case '-':val = n1 - n2;
                break;
            case '*':val = n1 * n2;
                break;
            case '/':val = n1 / n2;
                break;
            default: std::cerr << "getVal failed" << std::endl;
                exit(1);
        }

        if (val < 0) {
            fprintf(stderr, "%ld %c %ld = %ld\n", n1, act.op, n2, val);
            std::cerr << "val overflowed" << std::endl;
            exit(1);
        }
        return val;
    }

    static long getNewTarget(bool unkownLeft, long target, char op, long otherVal) {
        // position does not matter
        switch (op) {
            case '+':return target - otherVal;
            case '*':return target / otherVal;
        }

        if (unkownLeft) {
            switch (op) {
                case '/':
                    // target = x / otherVal
                    // x =
                    return target * otherVal;
                case '-':
                    // target = x - otherVal
                    // x =
                    return target + otherVal;
                default:std::cerr << "getNewTarget (left): op (" << op << ") not implemented" << std::endl;
                    exit(1);
            }
        } else {
            switch (op) {
                case '/':std::cerr << "Does this ever happen?" << std::endl;
                    exit(1);
                    return otherVal / target;
                case '-':return otherVal - target;
                default: std::cerr << "getNewTarget (right): op (" << op << ") not implemented" << std::endl;
                    exit(1);
            }
        }
    }

    static long getValHumn(std::string name, long target) {
        if (name == "humn")
            return target;

        Monkey m = monkeys[name];
        if (m.isNumber) {
            std::cerr << "isNumber not for getValHumn" << std::endl;
            exit(1);
        }

        Action act = m.act;
        std::string m1 = act.m1;
        std::string m2 = act.m2;

        std::string depends;
        std::string other;
        if (dependOnHumn.count(m1) > 0) {
            depends = m1;
            other = m2;
        } else {
            depends = m2;
            other = m1;
        }
        long otherVal = getVal(other);

        bool unkownLeft = depends == m1;

        long newTarget = getNewTarget(unkownLeft, target, act.op, otherVal);

        return getValHumn(depends, newTarget);
    }

    static long getValRoot() {
        Monkey m = monkeys["root"];
        Action act = m.act;
        std::string m1 = act.m1;
        std::string m2 = act.m2;

        std::string depends;
        std::string other;
        if (dependOnHumn.count(m1) > 0) {
            depends = m1;
            other = m2;
        } else {
            depends = m2;
            other = m1;
        }
        long target = getVal(other);

        return getValHumn(depends, target);
    }

    static void mark() {
        for (auto item: monkeys) {
            std::string name = item.first;
            Monkey m = item.second;
            if (m.isNumber || dependOnHumn.count(name) > 0)
                continue;

            Action act = m.act;
            std::string m1 = act.m1;
            std::string m2 = act.m2;

            if (dependOnHumn.count(m1) > 0 || dependOnHumn.count(m2) > 0) {
                dependOnHumn.insert(m.name);
                mark();
            }
        }
    }

    static bool both() {
        for (auto item: monkeys) {
            std::string name = item.first;
            Monkey m = item.second;
            if (m.isNumber)
                continue;

            Action act = m.act;
            std::string m1 = act.m1;
            std::string m2 = act.m2;

            if (dependOnHumn.count(m1) > 0 && dependOnHumn.count(m2) > 0) {
                return true;
            }
        }

        return false;
    }

    static void part1(std::string_view sv) {
        std::ifstream file(sv.data());
        std::string line;
        while (getline(file, line)) {
            Monkey m = parseMonkey(line);
            monkeys[m.name] = m;
        }
        printf("Part1: %ld\n", getVal("root"));
    }

    static void part2(std::string_view sv) {
        std::ifstream file(sv.data());
        std::string line;
        while (getline(file, line)) {
            Monkey m = parseMonkey(line);
            monkeys[m.name] = m;
        }
        dependOnHumn.insert("humn");
        mark();
        if (both()) {
            std::cerr << "Can't be both" << std::endl;
        }
        printf("Part2: %ld\n", getValRoot());
    }
}

int main() {
    Day21::part1("Input1.txt");
    Day21::part2("Input1.txt");
}