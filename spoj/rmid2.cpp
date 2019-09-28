#include <iostream>
#include <vector>
#include <algorithm>

void _main();

int main() {
    std::ios::sync_with_stdio(false);
    _main();
}

namespace input
{
    template<class T> T read() {
        T x;
        std::cin >> x;
        return x;
    }
    template<class T> void scanList(std::vector<T>& list, size_t size) {
        list.resize(size);
        for (int i = 0; i < size; ++ i)
            std::cin >> list[i];
    }
    struct Input
    {
        std::vector<int> list;
        void read() {
            int number;
            while (std::cin >> number, number != 0) {
                list.push_back(number);
            }
        }
    };
    Input read() {
        Input input;
        input.read();
        return input;
    }
}

namespace heap {
    template<class T, class C = std::less<T>>
    class Picker {
    public:
        Picker(C _comp = C{}) : comp(_comp) {} 
        void push(T const& t) {
            data.push_back(t);
            std::push_heap(data.begin(), data.end(), comp);
        }
        void multiPush(std::vector<T> const& tList) {
            for (auto const& t : tList) push(t);
        }
        T const& pick() const { return data.front(); }
        bool isEmpty() const { return data.empty(); }
        void pop() {
            std::pop_heap(data.begin(), data.end(), comp);
            data.pop_back();
        }
        std::vector<T> const& getData() const { return data; }
    private:
        std::vector<T> data;
        C comp;
    };

    template<class T>
    class MedianComputer {
        heap::Picker<T, std::greater<T> > upper{};
        heap::Picker<T> lower{};
        size_t size{0};
    public:
        T const& compute() {
            return lower.pick();
        }
        void pop() {
            if (size % 2 == 1) {
                lower.pop();
            } else {
                lower.pop();
                auto min = upper.pick();
                upper.pop();
                lower.push(min);
            }
            size --;
        }
        void push(T const& node) {
            if (size % 2 == 1) {
                if (not lower.isEmpty() and node < lower.pick()) {
                    lower.push(node);
                    auto max = lower.pick();
                    lower.pop();
                    upper.push(max);
                } else {
                    upper.push(node);
                }
            } else {
                if (not upper.isEmpty() and node > upper.pick()) {
                    upper.push(node);
                    auto min = upper.pick();
                    upper.pop();
                    lower.push(min);
                } else {
                    lower.push(node);
                }
            }
            size ++;
        }
    };
}


class Solver
{
    input::Input input;
public:
    void solve() {
        input.read();
        heap::MedianComputer<int> medianComputer;
        for (int x : input.list) {
            if (x == -1) {
                std::cout << medianComputer.compute() << std::endl;
                medianComputer.pop();
            } else {
                medianComputer.push(x);
            }
        }
    }
};

void _main() {
    auto nbTests = input::read<int>();
    for (int i = 0; i < nbTests; ++ i) {
        Solver().solve();
    }
}

