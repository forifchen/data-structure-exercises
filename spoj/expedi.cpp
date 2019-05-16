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
    template<class T> void readList(std::vector<T>& list, size_t size) {
        list.resize(size);
        for (int i = 0; i < size; ++ i)
            list[i].read();
    }
    struct Input
    {
        struct Stop {
            int distanceToTown;
            int fuel;
            void read() {
                std::cin >> distanceToTown >> fuel;
            }
        };
        int nbStops;
        std::vector<Stop> stopList;
        Stop start;
        void read() {
            std::cin >> nbStops;
            readList(stopList, nbStops);
            start.read();
        }
    };
    Input read() {
        Input input;
        input.read();
        return input;
    }
}

struct DistanceComparator {
    std::vector<input::Input::Stop> *pStopList;
    bool operator()(int x, int y) const {
        return (*pStopList)[x].distanceToTown < (*pStopList)[y].distanceToTown;
    }
};
struct FuelComparator {
    std::vector<input::Input::Stop> *pStopList;
    bool operator()(int x, int y) const {
        return (*pStopList)[x].fuel < (*pStopList)[y].fuel;
    }
};

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
}


class Solver
{
    input::Input input;
public:
    int solve() {
        input.read();
        auto picker = heap::Picker<int, FuelComparator>(FuelComparator{&input.stopList});

        input.nbStops ++;
        input.stopList.push_back(input::Input::Stop{0, 0});

        std::vector<int> indices(input.nbStops);
        for (int i = 0; i < input.nbStops; ++ i) indices[i] = i;
        std::sort(indices.rbegin(), indices.rend(), DistanceComparator{&input.stopList});

        int nbNeededStops = 0;
        int currentFuel = input.start.fuel;
        int currentDistance = input.start.distanceToTown;
        for (int index : indices) {
            auto& stop = input.stopList[index];

            while (not picker.isEmpty() && currentDistance - stop.distanceToTown > currentFuel) {
                nbNeededStops ++;
                auto& stopToUse = input.stopList[picker.pick()];
                picker.pop();
                currentFuel += stopToUse.fuel;
            }
            if (currentDistance - stop.distanceToTown > currentFuel) return -1;

            picker.push(index);
            currentFuel -= currentDistance - stop.distanceToTown;
            currentDistance = stop.distanceToTown;
        }
        return nbNeededStops;
    }
};

void _main() {
    auto nbTests = input::read<int>();
    for (int i = 0; i < nbTests; ++ i) {
        std::cout << Solver().solve() << std::endl;
    }
}


