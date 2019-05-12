#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

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
        struct Task {
            int savingTimePerDollar;
            int estimatedTime;
            int deadline;
            void read() {
                std::cin >> savingTimePerDollar
                    >> estimatedTime
                    >> deadline;
            }
        };
        int nbTasks;
        std::vector<Task> tasks;
        void read() {
            std::cin >> nbTasks;
            readList(tasks, nbTasks);
        }
    };
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
        double res;
        void print() {
            std::cout << std::fixed << std::setprecision(2) << res << std::endl;
        }
    };
    
}

struct DeadlineComparator {
    input::Input* pInput;
    DeadlineComparator(input::Input* _pInput) :
        pInput(_pInput) {}
    bool operator()(int x, int y) {
        return pInput->tasks[x].deadline <
            pInput->tasks[y].deadline;
    }
};
struct CheapnessComparator {
    input::Input* pInput;
    CheapnessComparator(input::Input* _pInput) :
        pInput(_pInput) {}
    bool operator()(int x, int y) {
        return pInput->tasks[x].savingTimePerDollar <
            pInput->tasks[y].savingTimePerDollar;
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
    output::Output solve() {
        input.read();
        std::vector<int> indices(input.nbTasks);
        for (int i = 0; i < input.nbTasks; ++ i) {
            indices[i] = i;
        }
        std::sort(indices.begin(), indices.end(), DeadlineComparator(&input));
        auto picker = heap::Picker<int, CheapnessComparator>(
            CheapnessComparator(&input)
        );
        std::vector<int> savingTimePerTaskId(input.nbTasks, 0);
        int accumulatedTime = 0;

        for (int index : indices) {
            auto& task = input.tasks[index];
            picker.push(index);
            accumulatedTime += task.estimatedTime;

            if (accumulatedTime > task.deadline) {
                while(true) {
                    auto cheapestTaskId = picker.pick();
                    auto currentTime = input.tasks[cheapestTaskId].estimatedTime -
                        savingTimePerTaskId[cheapestTaskId];
                    if (accumulatedTime - currentTime <= task.deadline) {
                        savingTimePerTaskId[cheapestTaskId] += accumulatedTime - task.deadline;
                        accumulatedTime = task.deadline;
                        break;
                    } else {
                        picker.pop();
                        savingTimePerTaskId[cheapestTaskId] += currentTime;
                        accumulatedTime -= currentTime;
                    }
                }
            }
        }
        double res = 0;
        for (int i = 0; i < input.nbTasks; ++ i) {
            res += double(savingTimePerTaskId[i]) / input.tasks[i].savingTimePerDollar;
        }
        return { res };
    }
};

void _main() {
    auto nbTests = input::read<int>();
    for (int i = 0; i < nbTests; ++ i) {
        Solver().solve().print();
    }
}

