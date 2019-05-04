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
        struct Query {
            int x, y;
            void read() {
                std::cin >> x >> y;
            }
        };
        int n;
        std::vector<int> list;
        int nbQueries;
        std::vector<Query> queryList;
        void read() {
            std::cin >> n;
            scanList(list, n);
            std::cin >> nbQueries;
            scanList(queryList, nbQueries);
        }
    };
    Input read() {
        Input input;
        input.read();
        return input;
    }
}
namespace std {
    std::istream& operator>>(std::istream& is, input::Input::Query & query) {
        query.read();
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
        std::vector<int> answers;
        void print() {
            printList(answers);
        }
    };
    
}

namespace segment_tree {
    template<class Node>
    class SegmentTree {
        int size;
        std::vector<Node> range;
        Node reduceLeft(Node& res, std::vector<int> const& nodeIds, bool& isFirst) const {
            for (auto& nodeId : nodeIds) {
                if (isFirst) {
                    res = range[nodeId];
                    isFirst = false;
                } else {
                    res = merge(res, range[nodeId]);
                }
            }
        }
        Node reduceRight(Node& res, std::vector<int> const& nodeIds, bool& isFirst) const {
            for (auto it = nodeIds.rbegin(); it != nodeIds.rend(); ++ it) {
                if (isFirst) {
                    res = range[*it];
                    isFirst = false;
                } else {
                    res = merge(res, range[*it]);
                }
            }
        }
        virtual Node merge(Node const& lhs, Node const& rhs) const = 0;
        virtual Node buildLeaf(int index) = 0;
    public:
        Node rangeQuery(int from, int to) const {
            std::vector<int> leftNodes, rightNodes;
            for (from += size, to += size; from < to; from >>= 1, to >>= 1) {
                if (from & 1) {
                    leftNodes.push_back(from);
                    from ++;
                }
                if (to & 1) {
                    to --;
                    rightNodes.push_back(to);
                }
            }
            Node res;
            bool isFirst = true;
            reduceLeft(res, leftNodes, isFirst);
            reduceRight(res, rightNodes, isFirst);
            return res;
        }
        void init(int n) {
            size = n;
            range.resize(n * 2);
            for (int i = 0; i < size; ++ i) range[i + size] = buildLeaf(i);
            for (int i = size - 1; i >= 1; -- i) {
                range[i] = merge(range[i*2], range[i*2 + 1]);
            }
        }
    };
}

struct Node {
    int bestPrefix, bestSuffix, bestSum, sum;
};

class SegmentTree : public segment_tree::SegmentTree<Node> {
    Node buildLeaf(int index) override {
        return { list[index], list[index], list[index], list[index] };
    }
    Node merge(Node const& lhs, Node const& rhs) const {
        return {
            std::max(lhs.bestPrefix, lhs.sum + rhs.bestPrefix),
            std::max(lhs.bestSuffix + rhs.sum, rhs.bestSuffix),
            std::max(lhs.bestSuffix + rhs.bestPrefix, std::max(lhs.bestSum, rhs.bestSum)),
            lhs.sum + rhs.sum
        };
    }
    std::vector<int> list;
public:
    SegmentTree(std::vector<int> _list) {
        list = std::move(_list);
        init(list.size());
    }
};


class Solver
{
public:
    output::Output solve(input::Input input) {
        auto segmentTree = SegmentTree(input.list);
        std::vector<int> res;
        for (auto& query : input.queryList) {
            res.push_back(segmentTree.rangeQuery(query.x - 1, query.y).bestSum);
        }
        return { std::move(res) };
    }
};

void _main() {
    Solver().solve(input::read()).print();
}

