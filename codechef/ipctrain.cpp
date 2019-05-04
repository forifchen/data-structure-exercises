#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <queue>

template<typename T> T read() {
    T x;
    std::cin >> x;
    return x;
}
using Sadness = int64_t;
struct Input {
    Input(Input const&) = delete;
    Input& operator=(Input const&) = delete;
    Input(Input &&) = default;

    struct Trainer {
        Trainer(Trainer const&) = delete;
        Trainer& operator=(Trainer const&) = delete;
        Trainer(Trainer &&) = default;
        Trainer(int a, int nb, Sadness const& sadness) :
            arrivalDay(a),
            nbDesiredLectures(nb),
            sadnessByMissedLecture(sadness) {}

        const int arrivalDay;
        const int nbDesiredLectures;
        const Sadness sadnessByMissedLecture;
    };
    const int nbTrainers;
    const int nbDays;
    std::vector<Trainer> trainerList;

    Input(int nbTrainers, int nbDays, std::vector<Trainer>&& trainerList) :
        nbTrainers(nbTrainers), nbDays(nbDays), trainerList(std::move(trainerList)) {}
};
template<>
Input::Trainer read() {
    auto arrivalDay = read<int>() - 1;
    auto nbDesiredLectures = read<int>();
    auto sadness = read<Sadness>();
    return Input::Trainer(
        arrivalDay,
        nbDesiredLectures,
        sadness
    );
}
template<>
Input read() {
    auto nbTrainers = read<int>();
    auto nbDays = read<int>();
    auto trainers = std::vector<Input::Trainer>{};
    for (int i = 0; i < nbTrainers; ++ i) {
        trainers.push_back(read<Input::Trainer>());
    }
    return Input(
        nbTrainers,
        nbDays,
        std::move(trainers)
    );
}
namespace output {
    struct Output { Sadness minimalSadness; };
    struct Trainer {
        Trainer(Trainer const&) = delete;
        Trainer& operator=(Trainer const&) = delete;
        Trainer(Trainer &&) = default;
        Trainer& operator=(Trainer &&) = default;
        Trainer(Sadness const& sadness, int nb) : sadnessByMissedLecture(sadness), nbMissedLectures(nb) {}

        Sadness sadnessByMissedLecture;
        mutable int nbMissedLectures;
    };

    class TrainerComparator {
    public:
        bool operator()(Trainer const& lhs, Trainer const& rhs) const {
            return lhs.sadnessByMissedLecture < rhs.sadnessByMissedLecture;
        }
    };
    void print(Output const& output) {
        std::cout << output.minimalSadness << std::endl;
    }
}
namespace heap {
    template<typename T, typename C = std::less<T>>
    class Picker {
    public:
        void push(T&& t) {
            data.push_back(std::move(t));
            std::push_heap(data.begin(), data.end(), C());
        }
        void multiPush(std::vector<T>&& tList) {
            for (auto& t : tList) push(std::move(t));
        }
        T const& pick() const { return data.front(); }
        bool isEmpty() const { return data.empty(); }
        void pop() {
            std::pop_heap(data.begin(), data.end(), C());
            data.pop_back();
        }
        std::vector<T> const& getData() const { return data; }
    private:
        std::vector<T> data;
    };
}
class Solver {
public:
    output::Output solve(Input const& input) {
        auto trainerPicker = TrainerPicker();
        auto trainersByDay = buildTrainersByDay(input.nbDays, input.trainerList);

        for (int day = 0; day < input.nbDays; ++ day) {
            trainerPicker.multiPush(std::move(trainersByDay[day]));
            if (! trainerPicker.isEmpty()) {
                auto& trainer = trainerPicker.pick();
                trainer.nbMissedLectures --;
                if (trainer.nbMissedLectures == 0) {
                    trainerPicker.pop();
                }
            }
        }
        return output::Output{getSadness(trainerPicker)};
    }
private:
    using TrainerPicker = heap::Picker<output::Trainer, output::TrainerComparator>;

    Sadness getSadness(TrainerPicker const& trainerPicker) const {
        Sadness sadness = 0;
        for (auto const& trainer : trainerPicker.getData()) {
            sadness += trainer.sadnessByMissedLecture * trainer.nbMissedLectures;
        }
        return sadness;
    }

    std::vector<std::vector<output::Trainer>>
    buildTrainersByDay(int nbDays, std::vector<Input::Trainer> const& trainerList) const {
        std::vector<std::vector<output::Trainer>> res(nbDays);
        for (auto const& trainer : trainerList) {
            res[trainer.arrivalDay].push_back(buildTrainer(trainer));
        }
        return res;
    }
    output::Trainer buildTrainer(Input::Trainer const& trainer) const {
        return output::Trainer(
            trainer.sadnessByMissedLecture,
            trainer.nbDesiredLectures
        );
    }
};

void solve() {
    auto nbTests = read<int>();
    auto solver = Solver();
    for (int i = 0; i < nbTests; ++ i) {
        output::print(solver.solve(read<Input>()));
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    solve();
}
