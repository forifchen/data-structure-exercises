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
        void read() {

        }
    };
    Input read() {
        Input input;
        input.read();
        return input;
    }
}
namespace heap {
    template<typename T, typename C>
    class Picker {
        public:
            void push(T t) {
                data.push_back(t);
                std::push_heap(data.begin(), data.end(), C());
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
    void process() {
        auto type = input::read<int>();
        if (type == 1) {
            addReview(input::read<int>());
        } else if (type == 2) {
            if (visibleRatings.isEmpty()) {
                std::cout << "No reviews yet" << std::endl;
            } else {
                std::cout << visibleRatings.pick() << std::endl;
            }
        }
    }
private:
    heap::Picker<int, std::greater<int>> visibleRatings;
    heap::Picker<int, std::less<int>> invisibleRatings;

    size_t size() {
        return visibleRatings.getData().size() + invisibleRatings.getData().size();
    }
    void addReview(int rating) {
        auto length = size();
        if (length % 3 <= 1) {
            visibleRatings.push(rating);
            int min = visibleRatings.pick();
            visibleRatings.pop();
            invisibleRatings.push(min);
        } else {
            invisibleRatings.push(rating);
            int max = invisibleRatings.pick();
            invisibleRatings.pop();
            visibleRatings.push(max);
        }
    }
};

void _main() {
    auto nbQueries = input::read<int>();
    Solver solver;
    for (int i = 0; i < nbQueries; ++ i) {
        solver.process();
    }
}
