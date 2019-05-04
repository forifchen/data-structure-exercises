#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

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
        int n;
        std::vector<std::string> grid;
        void read() {
            std::cin >> n;
            scanList(grid, n);
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
        std::string answer;
        void print() {
            std::cout << answer << std::endl;
        }
    };
    
}

namespace bfs {
    template<class Node>
    class BFS {
    public:
        bool findPath(Node from, Node to) {
            std::queue<Node> nodeList;
            mark(from);
            nodeList.push(std::move(from));
            while (! nodeList.empty()) {
                auto node = nodeList.front();
                nodeList.pop();
                if (node == to) return true;
                for (auto& neighbor: getNeighbors(node)) {
                    if (isMarked(neighbor)) continue;
                    mark(neighbor);
                    nodeList.push(std::move(neighbor));
                }
            }
            return false;
        }
    private:
        virtual void mark(Node const&) = 0;
        virtual bool isMarked(Node const&) = 0;
        virtual std::vector<Node> getNeighbors(Node const&) = 0;
    };
}

struct Cell {
    int x, y;
};
bool operator==(Cell const& lhs, Cell const& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}
using Marker = std::vector<std::vector<bool>>;
class BFS final : public bfs::BFS<Cell> {
public:
    BFS(std::vector<std::string> const& grid) {
        int n = grid.size(), m = grid.front().size();
        marker.resize(n);
        for (int i = 0; i < n; ++ i) marker[i].resize(m);

        for (int i = 0; i < n; ++ i) {
            for (int j = 0; j < m; ++ j) {
                marker[i][j] = grid[i][j] == '#';
            }
        }
    }
private:
    Marker marker;
    void mark(Cell const& cell) {
        marker[cell.x][cell.y] = true;
    }
    bool isMarked(Cell const& cell) {
        return marker[cell.x][cell.y];
    }
    std::vector<Cell> getNeighbors(Cell const& cell) {
        static std::vector<Cell> directions{
            {1, 0},
            {0, -1},
            {-1, 0},
            {0, 1}
        };
        std::vector<Cell> list;
        std::for_each(directions.begin(), directions.end(), [&, this](Cell const& direction) {
            Cell neighbor{cell.x + direction.x, cell.y + direction.y};
            if (isInside(neighbor)) {
                list.push_back(std::move(neighbor));
            }
        });
        return list;
    }
    bool isInside(Cell const& cell) {
        static int n = marker.size();
        static int m = marker.front().size();
        return 0 <= cell.x && cell.x < n &&
            0 <= cell.y && cell.y < m;
    }
};

class PathCounter {
    int n, m;
    static int MOD;
    std::vector<std::vector<int64_t>> counter;
public:
    PathCounter(std::vector<std::string> const& grid) {
        n = grid.size();
        m = grid.front().size();
        counter.resize(n);
        for (int i = 0; i < n; ++ i) {
            counter[i].resize(m);
            for (int j = 0; j < m; ++ j) {
                if (i == 0 && j == 0) counter[i][j] = 1;
                else {
                    int64_t &res = counter[i][j] = 0;
                    if (grid[i][j] == '.') {
                        if (i) res += counter[i-1][j];
                        if (j) res += counter[i][j - 1];
                    }
                    if (res > MOD) res -= MOD;
                }
            }
        }
    }
    int64_t count() {
        return counter[n-1][m-1] % MOD;
    }
};
int PathCounter::MOD = (1 << 31) - 1;

class Solver
{
public:
    output::Output solve(input::Input input) {
        int n = input.n;
        std::vector<std::string>& grid = input.grid;
        if( ! BFS(grid).findPath({0,0}, {n-1,n-1}) ) {
            return { "INCONCEIVABLE" };
        }
        auto count = PathCounter(grid).count();
        if (count == 0) return { "THE GAME IS A LIE" };
        return { std::to_string(count) };
    }
};

void _main() {
    Solver().solve(input::read()).print();
}

