#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include "dfa.hpp"

// Initialize the parser for the DFA problem
void DFAProblem::initialize_parser(cxxopts::Options &options) {
    options.add_options()
        ("check", "Check words with the DFA",
         cxxopts::value<std::string>());
}

// Check if the DFA problem is chosen
bool DFAProblem::is_chosen_problem(const cxxopts::ParseResult &args) {
    return args.count("check") > 0;
}

// Read the DFA from the input file
void DFAProblem::readDFA() {
    std::ifstream inputFile(inputFilename);

    std::string line;

    // 1. line: states
    std::getline(inputFile, line);

    {
        std::stringstream ss(line);
        std::string state;

        while (ss >> state) {
            states.insert(state);
        }
    }

    // 2. line: alphabet
    std::getline(inputFile, line);

    {
        std::stringstream ss(line);
        std::string symbol;

        while (ss >> symbol) {
            alphabet.insert(symbol);
        }
    }

    // 3. line: start state
    std::getline(inputFile, line);

    {
        std::stringstream ss(line);
        ss >> startState;
    }

    // 4. line: final states
    std::getline(inputFile, line);

    {
        std::stringstream ss(line);
        std::string state;

        while (ss >> state) {
            finalStates.insert(state);
        }
    }

    // Remaining lines: transitions
    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);

        std::string from;
        std::string symbol;
        std::string to;

        ss >> from >> symbol >> to;

        transitions[from][symbol] = to;
    }
}

// Check whether a word is accepted by the DFA
bool DFAProblem::accepts(const std::string& word) const {
    std::string currentState = startState;

    for (char c : word) {
        std::string symbol(1, c);

        auto stateIt = transitions.find(currentState);

        if (stateIt == transitions.end()) {
            return false;
        }

        auto transitionIt = stateIt->second.find(symbol);

        if (transitionIt == stateIt->second.end()) {
            return false;
        }

        currentState = transitionIt->second;
    }

    return finalStates.find(currentState) != finalStates.end();
}

// Write the results to the output file
void DFAProblem::writeResults() {
    std::ofstream outputFile(outputFilename);

    std::stringstream ss(checkWords);
    std::string word;

    bool first = true;

    while (std::getline(ss, word, ',')) {
        if (!first) {
            outputFile << '\n';
        }

        if (accepts(word)) {
            outputFile << "IGEN";
        } else {
            outputFile << "NEM";
        }

        first = false;
    }
}

// Run the DFA problem
int DFAProblem::run(const cxxopts::ParseResult &args) {
    inputFilename = args["input"].as<std::string>();
    outputFilename = args["output"].as<std::string>();
    checkWords = args["check"].as<std::string>();

    readDFA();
    writeResults();

    return 0;
}