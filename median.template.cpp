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

    template<class T>
    class MedianComputer {
        heap::Picker<T, std::greater<T> > upper{};
        heap::Picker<T> lower{};
        size_t size{0};
    public:
        T const& compute() {
            return lower.pick();
        }
        void push(T const& node) {
            if (size % 2 == 1) {
                if (not lower.isEmpty() and node < lower.pick()) {
                    lower.push(node);
                    auto max = lower.pick();
                    lower.pop();
                    upper.push(max);
                } else {
                    upper.push(node);
                }
            } else {
                if (not upper.isEmpty() and node > upper.pick()) {
                    upper.push(node);
                    auto min = upper.pick();
                    upper.pop();
                    lower.push(min);
                } else {
                    lower.push(node);
                }
            }
            size ++;
        }
    };
}
