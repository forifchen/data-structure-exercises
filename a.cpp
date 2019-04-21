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
    const std::vector<std::shared_ptr<Trainer>> trainerList;

    Input(int nbTrainers, int nbDays, std::vector<std::shared_ptr<Trainer>> const& trainerList) :
        nbTrainers(nbTrainers), nbDays(nbDays), trainerList(std::move(trainerList)) {}
};
std::shared_ptr<Input::Trainer> readTrainer() {
    auto arrivalDay = read<int>() - 1;
    auto nbDesiredLectures = read<int>();
    auto sadness = read<Sadness>();
    return std::make_shared<Input::Trainer>(
        arrivalDay,
        nbDesiredLectures,
        sadness
    );
}
std::shared_ptr<Input> readInput() {
    auto nbTrainers = read<int>();
    auto nbDays = read<int>();
    auto trainers = std::vector<std::shared_ptr<Input::Trainer>>{};
    for (int i = 0; i < nbTrainers; ++ i) {
        auto trainer = readTrainer();
        trainers.push_back(trainer);
    }
    return std::make_shared<Input>(
        nbTrainers,
        nbDays,
        trainers
    );
}
struct Output {
    struct Trainer {
        Trainer(Trainer const&) = delete;
        Trainer& operator=(Trainer const&) = delete;
        Trainer(Sadness const& sadness, int nb) : sadnessByMissedLecture(sadness), nbMissedLectures(nb) {}

        const Sadness sadnessByMissedLecture;
        int nbMissedLectures;
    };
    const Sadness minimalSadness;
};
void printOutput(Output const& output) {
    std::cout << output.minimalSadness << std::endl;
}
class Solver {
private:
    using TrainerPtr = std::shared_ptr<Output::Trainer>;
    class TrainerPicker {
    public:
        void push(TrainerPtr const& trainer) {
            data.push(trainer);
        }
        void multiPush(std::vector<TrainerPtr> const& trainers) {
            for (auto const& trainer : trainers) {
                push(trainer);
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
                return lhs->sadnessByMissedLecture < rhs->sadnessByMissedLecture;
            }
        };
        std::priority_queue<TrainerPtr, std::vector<TrainerPtr>, TrainerComparator> data;
    };
public:
    Output solve(Input const& input) {
        auto trainerPicker = TrainerPicker();
        auto trainersByDay = buildTrainersByDay(input.nbDays, input.trainerList);

        for (int day = 0; day < input.nbDays; ++ day) {
            trainerPicker.multiPush(trainersByDay[day]);
            if (! trainerPicker.isEmpty()) {
                auto& trainer = *trainerPicker.pick();
                trainer.nbMissedLectures --;
                if (trainer.nbMissedLectures == 0) {
                    trainerPicker.pop();
                }
            }
        }

        Sadness sadness = 0;
        for (auto const& trainerList : trainersByDay) {
            for (auto const& trainer : trainerList) {
                sadness += trainer->sadnessByMissedLecture * trainer->nbMissedLectures;
            }
        }
        return Output{sadness};
    }
private:
    std::vector<std::vector<TrainerPtr>>
    buildTrainersByDay(int nbDays, std::vector<std::shared_ptr<Input::Trainer>> const& trainerList) const {
        std::vector<std::vector<TrainerPtr>> res(nbDays);
        for (auto const& trainer : trainerList) {
            res[trainer->arrivalDay].push_back(buildTrainer(trainer));
        }
        return res;
    }
    TrainerPtr buildTrainer(std::shared_ptr<Input::Trainer> const& trainer) const {
        return std::make_shared<Output::Trainer>(
            trainer->sadnessByMissedLecture,
            trainer->nbDesiredLectures
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
