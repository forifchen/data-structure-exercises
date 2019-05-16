#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>

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
        int n, k;
        std::vector<int> list;
        void read() {
            std::cin >> k >> n;
            scanList(list, n);
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
        std::vector<int> list;
        void print() {
            std::accumulate(std::next(list.begin()), list.end(),
                std::ref(std::cout << list.front()),
                [](std::ostream& os, int value){
                    return std::ref(os << " " << value);
                }
            ).get() << std::endl;
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
        heap::Picker<int, std::greater<int>> picker;
        std::vector<int> list;
        for (int x : input.list) {
            picker.push(x);
            auto& data = picker.getData();
            if (data.size() < input.k) list.push_back(-1);
            else if (data.size() == input.k) list.push_back(picker.pick());
            else {
                picker.pop();
                list.push_back(picker.pick());
            }
        }
        return { std::move(list) };
    }
};

void _main() {
    auto nbTests = input::read<int>();
    for (int i = 0; i < nbTests; ++ i) {
        Solver().solve(input::read()).print();
    }
}
