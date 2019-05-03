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
        void read() {
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
    void printList(std::vector<int>& list) {
        if (list.empty()) return;
        std::cout << list.front();
        for (int i = 1; i < list.size(); ++ i)
            std::cout << " " << list[i];
        std::cout << std::endl;
    }
    struct Output
    {
        void print() {
        }
    };
    
}

class Solver
{
public:
    output::Output solve(input::Input const& input) {
    }
};

void _main() {
    auto nbTests = input::read<int>();
    for (int i = 0; i < nbTests; ++ i) {
        Solver().solve(input::read()).print();
    }
}

