#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

template<typename T>
T read() {
    T t;
    std::cin >> t;
    return t;
}
using Integer = int32_t;

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

        std::vector<Integer> list;
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

namespace heap {
    template<typename T, typename C = std::less<T>>
    class Picker {
    public:
        void push(T&& t) {
            data.push_back(std::move(t));
            std::push_heap(data.begin(), data.end(), C());
        }
        void multiPush(std::vector<T>&& tList) {
            for (auto& t : tList) push(std::move(t));
        }
        T const& pick() const { return data.front(); }
        bool isEmpty() const { return data.empty(); }
        void pop() {
            std::pop_heap(data.begin(), data.end(), C());
            data.pop_back();
        }
        std::vector<T> const& getData() const { return data; }
    private:
        std::vector<T> data;
    };
}

class Solver {
    using Picker = heap::Picker<int, std::greater<int>>;
public:
    output::Output solve(input::Input && input) {
        std::vector<Integer> list;
        list.reserve(input.setSize);

        auto& subsetSumList = input.subsetSumList;
        std::sort(subsetSumList.begin(), subsetSumList.end());

        std::vector<int> currentSubsetSums = {0};
        currentSubsetSums.reserve(1 << input.setSize);

        Picker sumPicker;
        sumPicker.push(0);

        for (int sum : subsetSumList) {
            if (! sumPicker.isEmpty()) {
                int expectedSum = sumPicker.pick();
                if (sum == expectedSum) {
                    sumPicker.pop();
                    continue;
                }
            }
            int element = sum;
            list.push_back(element);

            auto size = currentSubsetSums.size();
            for (int i = 0; i < size; ++ i) {
                int s = currentSubsetSums[i];
                currentSubsetSums.push_back(s + element);
                sumPicker.push(s + element);
            }
            sumPicker.pop();
        }
        return output::Output{ std::move(list) };
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
