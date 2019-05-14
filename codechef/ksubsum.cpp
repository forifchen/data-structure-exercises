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
        size_t n, k1, k2, k3;
        std::vector<int> list;
        void read() {
            std::cin >> n >> k1 >> k2 >> k3;
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
        int k1th, k2th, k3th;
        void print() {
            std::cout << k1th << " " << k2th << " " << k3th << std::endl;
        }
    };
}

class Solver
{
public:
    output::Output solve(input::Input const& input) {
        std::vector<int> bestSums, bestSumsUntilHere;
        for (int num : input.list) {
            insertSorted(bestSumsUntilHere, 0);
            multiIncrement(bestSumsUntilHere, num);
            update(bestSums, bestSumsUntilHere);
        }
        return {
            bestSums[input.k1 - 1],
            bestSums[input.k2 - 1],
            bestSums[input.k3 - 1]
        };
    }
private:
    void insertSorted(std::vector<int>& list, int item) {
        auto position = std::upper_bound(list.begin(), list.end(), item, std::greater<int>());
        list.insert(position, item);
        crop(list);
    }
    void multiIncrement(std::vector<int>& list, int value) {
        for (int &item : list) item += value;
    }
    void update(std::vector<int>& bestSums, std::vector<int> const& sums) {
        auto sorted = merge(bestSums, sums);
        crop(sorted);
        bestSums = std::move(sorted);
    }
    std::vector<int> merge(std::vector<int> const& a, std::vector<int> const& b) {
        std::vector<int> res;
        std::merge(
            a.begin(), a.end(),
            b.begin(), b.end(),
            std::back_inserter(res),
            std::greater<int>()
        );
        return res;
    }
    void crop(std::vector<int> &list) {
        if (list.size() > 2012) {
            list.resize(2012);
        }
    }
};

void _main() {
    auto nbTests = input::read<int>();
    for (int i = 0; i < nbTests; ++ i) {
        Solver().solve(input::read()).print();
    }
}