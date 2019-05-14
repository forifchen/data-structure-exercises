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
        int64_t a, b, c, n;
        void read() {
            std::cin >> a >> b >> c >> n;
        }
    };
    Input read() {
        Input input;
        input.read();
        return input;
    }
}
namespace output
{
    struct Output
    {
        int64_t sum;
        void print() {
            std::cout << sum << std::endl;
        }
    };
    
}
namespace heap {
    template<typename T, typename C>
    class Picker {
    public:
        void push(T t) {
            data.push_back(std::move(t));
            std::push_heap(data.begin(), data.end(), C());
        }
        void multiPush(std::vector<T> tList) {
            for (auto& t : tList) push(std::move(t));
        }
        T const& pick() const { return data.front(); }
        bool isEmpty() const { return data.empty(); }
        void pop() {
            if (isEmpty()) return;
            std::pop_heap(data.begin(), data.end(), C());
            data.pop_back();
        }
        std::vector<T> const& getData() const { return data; }
    private:
        std::vector<T> data;
    };
}
class Solver
{
public:
    output::Output solve(input::Input const& input) {
        static int64_t mod = 1e9 + 7;
        heap::Picker<int, std::greater<int>> upper;
        heap::Picker<int, std::less<int>> lower;
        lower.push(1);
        int64_t sum = 1;
        for (int i = 2; i <= input.n; ++ i) {
            auto median = lower.pick();
            auto next = (input.a * median + input.b * i  + input.c) % mod;
            sum += next;

            if (i % 2 == 0) {
                lower.push(next);
                auto max = lower.pick();
                lower.pop();
                upper.push(max);
            } else {
                upper.push(next);
                auto min = upper.pick();
                upper.pop();
                lower.push(min);
            }
        }
        return { sum };
    }
};

void _main() {
    auto nbTests = input::read<int>();
    for (int i = 0; i < nbTests; ++ i) {
        Solver().solve(input::read()).print();
    }
}
