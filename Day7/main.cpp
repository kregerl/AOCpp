#include <iostream>
#include <string_view>
#include <fstream>
#include <utility>
#include <vector>
#include <unordered_map>
#include <stack>

#define TOTAL_DISK_SPACE 70000000
#define NEEDED_DISK_SPACE 30000000

namespace Day7 {

    struct PseudoFile {
        int size;

        explicit PseudoFile(int size): size(size) {}
    };

    struct Node {
        Node* parent;
        std::vector<PseudoFile> files;
        std::unordered_map<std::string, Node*> dirs;

        explicit Node(Node* parent): parent(parent) {}

        int get_dir_size() {
            int sum = 0;
            for (auto& file : files) {
                sum += file.size;
            }
            for (const auto& dir : dirs) {
                sum += dir.second->get_dir_size();
            }
            return sum;
        }
    };

    static Node* create_directory_tree(std::string_view sv) {
        std::ifstream file(sv.data());

        Node* root = nullptr;
        Node* current = root;
        for (std::string line; std::getline(file, line);) {
            if (line[0] == '$') {
                size_t idx;
                std::string command = "cd";
                if ((idx = line.find(command)) != std::string::npos) {
                    // Is cd
                    auto path = line.substr(command.length() + idx + 1);
                    if (!current) {
                        root = new Node(nullptr);
                        current = root;
                    } else {
                        if (current->dirs.count(path)) {
                            current = current->dirs.at(path);
                        } else if (path == "/") {
                            current = root;
                        } else if (path == ".." && current->parent) {
                            current = current->parent;
                        } else {
                            std::cout << "No child dir named: " << path << std::endl;
                        }
                    }

                } else {
                    // Skip 'ls' commands
                    continue;
                }
            } else {
                const std::string type = "dir";
                bool is_dir = line.substr(0, 3) == type;
                std::string dir_name = line.substr(4);
                if (is_dir) {
                    current->dirs.insert({dir_name, new Node(current)});
                } else {
                    auto index = line.find(' ');
                    auto size = std::stoi(line.substr(0, index));
                    current->files.emplace_back(PseudoFile(size));
                }
            }
        }
        return root;
    }

    static void part1(std::string_view sv) {
        Node* root = create_directory_tree(sv);
        if (!root) {
            std::cout << "Root node should not be null" << std::endl;
            return;
        }
        int result = 0;
        std::stack<Node*> unvisited_nodes;
        unvisited_nodes.push(root);
        while (!unvisited_nodes.empty()) {
            size_t n = unvisited_nodes.size();
            while (n > 0) {
                Node* node = unvisited_nodes.top();
                unvisited_nodes.pop();
                auto dir_size = node->get_dir_size();
                if (dir_size <= 100000)
                    result += dir_size;
                for (const auto& dir : node->dirs)
                    unvisited_nodes.push(dir.second);

                n--;
            }
        }
        printf("Part1: %d\n", result);
    }

    static void part2(std::string_view sv) {
        Node* root = create_directory_tree(sv);
        if (!root) {
            std::cout << "Root node should not be null" << std::endl;
            return;
        }
        int unused_space = TOTAL_DISK_SPACE - root->get_dir_size();
        int smallest_dir_size_to_delete = root->get_dir_size();
        std::stack<Node*> unvisited_nodes;
        unvisited_nodes.push(root);
        while (!unvisited_nodes.empty()) {
            size_t n = unvisited_nodes.size();
            while (n > 0) {
                Node* node = unvisited_nodes.top();
                unvisited_nodes.pop();
                auto dir_size = node->get_dir_size();
                if (dir_size + unused_space >= NEEDED_DISK_SPACE)
                    smallest_dir_size_to_delete = std::min(smallest_dir_size_to_delete, dir_size);
                for (const auto& dir : node->dirs)
                    unvisited_nodes.push(dir.second);

                n--;
            }
        }
        printf("Part2: %d\n", smallest_dir_size_to_delete);
    }
}

int main() {
    Day7::part1("Input1.txt");
    Day7::part2("Input1.txt");
}
