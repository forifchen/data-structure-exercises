#include <iostream>
#include <algorithm>
#include <tuple>
#include <unordered_map>
#include <memory>

template<typename T>
T read() {
    T t;
    std::cin >> t;
    return t;
}
using Integer = std::unique_ptr<int>;

namespace input {
    struct Input {
        Input(Input const&) = delete;
        Input& operator=(Input const&) = delete;
        Input(Input &&) = default;
        Input& operator=(Input &&) = default;

        int setSize;
        std::vector<Integer> subsetSumList;
    };
    void generate() {
        std::vector<int> list;
        for (int i = 0; i < 15; ++ i) {
            list.push_back(std::rand() % 1000000);
        }
        std::cout << list.size() << std::endl;
        for (int mask = 0; mask < (1 << 15); ++ mask) {
            int sum = 0;
            for (int i = 0; i < 15; ++ i) {
                sum += (mask & (1 << i)) ? list[i] : 0;
            }
            std::cout << sum << " ";
        }
        std::cout << std::endl;
    }
}
template<>
input::Input read() {
    auto setsize = read<int>();
    auto list = std::vector<Integer>{};
    for (int i = 0; i < (1 << setsize); ++ i) {
        list.push_back(std::make_unique<int>(read<int>()));
    }
    return input::Input {
        setsize,
        std::move(list)
    };
}

namespace output {
    struct Output {
        Output(Output const&) = delete;
        Output& operator=(Output const&) = delete;
        Output(Output &&) = default;
        Output& operator=(Output &&) = default;

        std::vector<Integer> list;
    };
    template<typename T>
    void printList(std::vector<T> const& list) {
        for (int i = 0; i < list.size(); ++ i) {
            std::cout << *list[i] << char(i + 1 == list.size()? 10 : 32);
        }
    }
    void print(Output const& output) {
        printList(output.list);
    }
}


class Solver {
public:
    output::Output solve(input::Input && input) {
        std::vector<Integer> list;
        std::vector<Integer> subsetSumList = std::move(input.subsetSumList);
        std::sort(subsetSumList.begin(), subsetSumList.end(), [](auto & lhs, auto& rhs){ return *lhs < *rhs; });

        for (int i = 0; i < input.setSize; ++ i) {
            int minElement;
            std::tie(minElement, subsetSumList) = split(std::move(subsetSumList));
            list.push_back(std::make_unique<int>(minElement));
        }
        return output::Output{ std::move(list) };
    }
private:
    std::tuple<int, std::vector<Integer>>
    split(std::vector<Integer> &&list) {
        std::vector<Integer> reducedList;
        int minElement = *list[1];
        std::unordered_map<int, int> available;
        for (auto& x : list) available[*x] ++;
        for (auto& x : list) {
            if (available[*x] == 0) continue;
            available[*x] --;
            available[*x + minElement] --;
            reducedList.push_back(std::move(x));
        }
        return std::make_tuple(minElement, std::move(reducedList));
    }
};

void _main() {
    auto nbTests = read<int>();
    auto solver = Solver{};
    for (int i = 0; i < nbTests; ++ i) {
        output::print(solver.solve(read<input::Input>()));
    }
    if (nbTests > 0) return;
    std::cout << 50 << std::endl;
    for (int i = 0; i < 50; ++ i)
        input::generate();
}

int main() {
    _main();
}
