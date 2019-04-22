#include <iostream>
#include <algorithm>
#include <vector>
#include <tuple>
#include <unordered_map>

template<typename T>
T read() {
    T t;
    std::cin >> t;
    return t;
}

namespace input {
    struct Input {
        Input(Input const&) = delete;
        Input& operator=(Input const&) = delete;
        Input(Input &&) = default;
        Input& operator=(Input &&) = default;

        int setSize;
        std::vector<int> subsetSumList;
    };
    void generate() {
        std::vector<int> list;
        for (int i = 0; i < 15; ++ i) {
            list.push_back(std::rand() % 100);
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
    auto list = std::vector<int>{};
    for (int i = 0; i < (1 << setsize); ++ i) {
        list.push_back(read<int>());
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

        std::vector<int> list;
    };
    template<typename T>
    void printList(std::vector<T> const& list) {
        for (int i = 0; i < list.size(); ++ i) {
            std::cout << list[i] << char(i + 1 == list.size()? 10 : 32);
        }
    }
    void print(Output const& output) {
        printList(output.list);
    }
}

class Solver {
public:
    output::Output solve(input::Input && input) {
        std::vector<int> list;
        std::vector<int> subsetSumList = input.subsetSumList;
        std::sort(subsetSumList.begin(), subsetSumList.end());

        for (int i = 0; i < input.setSize; ++ i) {
            int minElement;
            std::tie(minElement, subsetSumList) = split(std::move(subsetSumList));
            list.push_back(minElement);
        }
        return output::Output{ list };
    }
private:
    std::tuple<int, std::vector<int>>
    split(std::vector<int> &&list) {
        std::vector<int> reducedList;
        int minElement = list[1];
        std::unordered_map<int, int> available;
        for (int x : list) available[x] ++;
        for (int x : list) {
            if (available[x] == 0) continue;
            reducedList.push_back(x);
            available[x] --;
            available[x + minElement] --;
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
    std::cout << 1 << std::endl;
    input::generate();
}

int main() {
    _main();
}
