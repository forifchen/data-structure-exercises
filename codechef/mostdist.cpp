#include <iostream>
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
    struct Point {
        int x, y;
        bool isRemoved;
        int f1() { return x + y; }
        int f2() { return -x - y; }
        int f3() { return x - y; }
        int f4() { return y - x; }
    };
    struct Comparator1 {
        std::vector<Point> *p;
        bool operator()(int x, int y) {
            return (*p)[x].f1() < (*p)[y].f1();
        }
    };
    struct Comparator2 {
        std::vector<Point> *p;
        bool operator()(int x, int y) {
            return (*p)[x].f2() < (*p)[y].f2();
        }
    };
    struct Comparator3 {
        std::vector<Point> *p;
        bool operator()(int x, int y) {
            return (*p)[x].f3() < (*p)[y].f3();
        }
    };
    struct Comparator4 {
        std::vector<Point> *p;
        bool operator()(int x, int y) {
            return (*p)[x].f4() < (*p)[y].f4();
        }
    };
    int answer;
    int nbQueries;
    std::vector<Point> pointList;
    heap::Picker<int, Comparator1> picker1{Comparator1{&pointList}};
    heap::Picker<int, Comparator2> picker2{Comparator2{&pointList}};
    heap::Picker<int, Comparator3> picker3{Comparator3{&pointList}};
    heap::Picker<int, Comparator4> picker4{Comparator4{&pointList}};

    void addPoint() {
        static int index = 0;

        int x, y;
        std::cin >> x >> y;
        x ^= answer;
        y ^= answer;

        pointList.push_back(Point{x, y, false});
        picker1.push(index);
        picker2.push(index);
        picker3.push(index);
        picker4.push(index);
        index ++;
    }
    void removePoint() {
        int index;
        std::cin >> index;
        index ^= answer;

        pointList[index - 1].isRemoved = true;
    }
    void updateMax(int &max, int &bestId, int value, int id) {
        if (max < value) {
            max = value;
            bestId = id;
        }
    }
    int getFarthestDistance() {
        int x, y;
        std::cin >> x >> y;
        x ^= answer;
        y ^= answer;

        while (true) {
            int bestDistance = -2e9 - 1, bestId;
            Point p{x, y, false};
            updateMax(bestDistance, bestId,
                pointList[picker1.pick()].f1() - p.f1(), 1);
            updateMax(bestDistance, bestId,
                pointList[picker2.pick()].f2() - p.f2(), 2);
            updateMax(bestDistance, bestId,
                pointList[picker3.pick()].f3() - p.f3(), 3);
            updateMax(bestDistance, bestId,
                pointList[picker4.pick()].f4() - p.f4(), 4);
            
            if (bestId == 1) {
                if (pointList[picker1.pick()].isRemoved) {
                    picker1.pop();
                } else {
                    return bestDistance;
                }
            } else if (bestId == 2) {
                if (pointList[picker2.pick()].isRemoved) {
                    picker2.pop();
                } else {
                    return bestDistance;
                }
            } else if (bestId == 3) {
                if (pointList[picker3.pick()].isRemoved) {
                    picker3.pop();
                } else {
                    return bestDistance;
                }
            } else if (bestId == 4) {
                if (pointList[picker4.pick()].isRemoved) {
                    picker4.pop();
                } else {
                    return bestDistance;
                }
            }
        }
        return -1;
    }
public:
    void solve() {
        std::cin >> nbQueries;
        for (int q = 0; q < nbQueries; ++ q) {
            char operation;
            std::cin >> operation;
            if (operation == '+') {
                addPoint();
            } else if (operation == '-') {
                removePoint();
            } else if (operation == '?') {
                answer = getFarthestDistance();
                std::cout << answer << "\n";
            }
        }
    }
};

void _main() {
    Solver().solve();
}

