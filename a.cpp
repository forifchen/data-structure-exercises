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
    const std::vector<Trainer> trainerList;

    Input(int nbTrainers, int nbDays, std::vector<Trainer>&& trainerList) :
        nbTrainers(nbTrainers), nbDays(nbDays), trainerList(std::move(trainerList)) {}
};
Input::Trainer readTrainer() {
    auto arrivalDay = read<int>() - 1;
    auto nbDesiredLectures = read<int>();
    auto sadness = read<Sadness>();
    return Input::Trainer(
        arrivalDay,
        nbDesiredLectures,
        sadness
    );
}
std::unique_ptr<Input> readInput() {
    auto nbTrainers = read<int>();
    auto nbDays = read<int>();
    auto trainers = std::vector<Input::Trainer>{};
    for (int i = 0; i < nbTrainers; ++ i) {
        auto trainer = readTrainer();
        trainers.push_back(std::move(trainer));
    }
    return std::make_unique<Input>(
        nbTrainers,
        nbDays,
        std::move(trainers)
    );
}
struct Output {
    struct Trainer {
        Trainer(Trainer const&) = delete;
        Trainer& operator=(Trainer const&) = delete;
        Trainer(Trainer &&) = default;
        Trainer& operator=(Trainer &&) = default;
        Trainer(Sadness const& sadness, int nb) : sadnessByMissedLecture(sadness), nbMissedLectures(nb) {}

        Sadness sadnessByMissedLecture;
        int nbMissedLectures;
    };
    const Sadness minimalSadness;
};
void printOutput(Output const& output) {
    std::cout << output.minimalSadness << std::endl;
}
class Solver {
private:
    using TrainerPtr = Output::Trainer;
    class TrainerPicker {
    public:
        void push(TrainerPtr && trainer) {
            data.push(std::move(trainer));
        }
        void multiPush(std::vector<TrainerPtr>&& trainers) {
            for (TrainerPtr& trainer : trainers) {
                data.push(std::move(trainer));
            }
        }
        TrainerPtr const& pick() const {
            return data.top();
        }
        bool isEmpty() const { return data.empty(); }
        void pop() {
            data.pop();
        }
    private:
        class TrainerComparator {
        public:
            bool operator()(TrainerPtr const& lhs, TrainerPtr const& rhs) const {
                return lhs.sadnessByMissedLecture < rhs.sadnessByMissedLecture;
            }
        };
        std::priority_queue<TrainerPtr, std::vector<TrainerPtr>, TrainerComparator> data;
    };
public:
    Output solve(Input const& input) {
        auto trainerPicker = TrainerPicker();
        auto trainersByDay = buildTrainersByDay(input.nbDays, input.trainerList);

        for (int day = 0; day < input.nbDays; ++ day) {
            trainerPicker.multiPush(std::move(trainersByDay[day]));
            if (! trainerPicker.isEmpty()) {
                auto& trainer = trainerPicker.pick();
                if (trainer.nbMissedLectures > 1) {
                    trainerPicker.push(TrainerPtr(trainer.sadnessByMissedLecture, trainer.nbMissedLectures - 1));
                }
                trainerPicker.pop();
            }
        }

        Sadness sadness = 0;
        while (! trainerPicker.isEmpty() ) {
            auto& trainer = trainerPicker.pick();
            sadness += trainer.sadnessByMissedLecture * trainer.nbMissedLectures;
            trainerPicker.pop();
        }
        return Output{sadness};
    }
private:
    std::vector<std::vector<TrainerPtr>>
    buildTrainersByDay(int nbDays, std::vector<Input::Trainer> const& trainerList) const {
        std::vector<std::vector<TrainerPtr>> res(nbDays);
        for (auto const& trainer : trainerList) {
            res[trainer.arrivalDay].push_back(buildTrainer(trainer));
        }
        return res;
    }
    TrainerPtr buildTrainer(Input::Trainer const& trainer) const {
        return Output::Trainer(
            trainer.sadnessByMissedLecture,
            trainer.nbDesiredLectures
        );
    }
};

void solve() {
    auto nbTests = read<int>();
    auto solver = Solver();
    for (int i = 0; i < nbTests; ++ i) {
        auto input = readInput();
        auto output = solver.solve(*input);
        printOutput(output);
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    solve();
}
