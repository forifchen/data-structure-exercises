#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

void _main();

int main() {
    std::ios::sync_with_stdio(false);
    _main();
}

namespace graph {
    template<class T>
    class Graph {
        public:
            virtual std::vector<T>const& getNeighbors(T node) const = 0;
    };
}

class Graph final : public graph::Graph<int> {
    std::vector<std::vector<int>> data;
    public:
        void read(int n, int m) {
            data.resize(n);
            for (int i = 0; i < m; ++ i) {
                int u, v;
                std::cin >> u >> v;
                u --; v --;
                data[u].push_back(v);
                data[v].push_back(u);
            }
        }
        std::vector<int> const& getNeighbors(int index) const {
            return data[index];
        }
};

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
        int n;
        std::vector<int> populationList;
        Graph tree;
        void read() {
            std::cin >> n;
            scanList(populationList, n);
            tree.read(n, n - 1);
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
        std::vector<int> capitalList;
        void print() {
            printList(capitalList);
        }
    };
}

namespace heap {
    template<typename T>
    class Picker {
    public:
        Picker(std::function<bool(T const&, T const&)> comp) : comp(comp) {
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
    private:
        std::vector<T> data;
        std::function<bool(T const&, T const&)> comp;
    };
}

class PopulationComparator {
    std::reference_wrapper<input::Input> input;
    public:
        PopulationComparator(input::Input& input) : input(input) {
        }
        bool operator()(int lhs, int rhs) const {
            return input.get().populationList[lhs] < input.get().populationList[rhs];
        }
};

using namespace std::placeholders;

class Solver
{
    using Picker = heap::Picker<int>;

    input::Input input;
    std::function<bool(int,int)> comp;
    Picker picker;

    bool comparePopulations(int lhs, int rhs) const {
        return input.populationList[lhs] < input.populationList[rhs];
    }
    int capitalNotIn(std::vector<int> const& infectedList, std::vector<int>& removed) {
        for (int i = 0; i < infectedList.size(); ++ i) {
            int infectedId = infectedList[i];
            int expectedId = picker.pick();
            if (infectedId != expectedId) return expectedId;
            picker.pop();
            removed.push_back(expectedId);
        }
        if (!picker.isEmpty()) return picker.pick();
        return -1;
    }

    int computeCapitalInfecting(int cityId) {
        std::vector<int> infectedList = input.tree.getNeighbors(cityId);
        infectedList.push_back(cityId);

        std::sort(infectedList.rbegin(), infectedList.rend(), comp);
        std::vector<int> removed;
        int res = capitalNotIn(infectedList, removed);
        picker.multiPush(removed);
        return res;
    }
public:
    Solver() :
        comp([this](int lhs, int rhs){ return comparePopulations(lhs, rhs); }),
        picker(comp)
    {
    }
    output::Output solve() {
        input.read();
        for (int i = 0; i < input.n; ++ i) {
            picker.push(i);
        }
        std::vector<int> capitalList(input.n);
        for (int i = 0; i < input.n; ++ i) {
            capitalList[i] = computeCapitalInfecting(i) + 1;
        }

        return { capitalList };
    }
};

void _main() {
    auto nbTests = input::read<int>();
    for (int i = 0; i < nbTests; ++ i) {
        Solver().solve().print();
    }
}

