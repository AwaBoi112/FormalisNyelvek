#ifndef PROBLEMS_DFA_H
#define PROBLEMS_DFA_H

#include "../problem.hpp"

#include <string>
#include <unordered_map>
#include <unordered_set>

class DFAProblem : public Problem {
private:
    std::unordered_set<std::string> states;
    std::unordered_set<std::string> alphabet;
    std::string startState;
    std::unordered_set<std::string> finalStates;

    // transitions[from][symbol] = to
    std::unordered_map<
        std::string,
        std::unordered_map<std::string, std::string>
    > transitions;

    std::string inputFilename;
    std::string outputFilename;
    std::string checkWords;

    void readDFA();
    bool accepts(const std::string& word) const;
    void writeResults();

public:
    void initialize_parser(cxxopts::Options &options) override;
    bool is_chosen_problem(const cxxopts::ParseResult &args) override;
    int run(const cxxopts::ParseResult &args) override;
};

#endif // PROBLEMS_DFA_H