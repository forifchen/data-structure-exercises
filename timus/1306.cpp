#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

void _main();

int main() {
    std::ios::sync_with_stdio(false);
    _main();
}

namespace heap {
    template<class T, class C = std::less<T>>
    class Picker {
    public:
        Picker(C _comp = C{}) : comp(_comp) {
        }
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
        std::vector<T> data;
        C comp;
    };
}


class Solver
{
public:
    void solve() {
        int n;
        std::cin >> n;
        heap::Picker<uint32_t> picker{};
        picker.data.reserve(n/2 + 5);
        int bound = n / 2 + 1;
        for (int i = 0; i < n; ++ i) {
            int x; std::cin >> x;
            if (i < bound) {
                picker.push(x);
            } else {
                picker.push(x);
                picker.pop();
            }
        }
        double median;
        if (n & 1) median = picker.pick();
        else {
            auto m1 = picker.pick();
            picker.pop();
            auto m2 = picker.pick();
            median = (m1 + m2) / 2.0;
        }
        std::cout << std::fixed << std::setprecision(1) << median << std::endl;

    }
};

void _main() {
    Solver().solve();
}


