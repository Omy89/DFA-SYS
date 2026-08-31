#include "DFA.h"

DFA::DFA() {
    initialState = "";
}

void DFA::addState(std::string state) {
    states.insert(state);
}

void DFA::addSymbol(std::string symbol) {
    alphabet.insert(symbol);
}

void DFA::setInitialState(std::string state) {
    initialState = state;
}

void DFA::addFinalState(std::string state) {
    finalStates.insert(state);
}

void DFA::addTransition(std::string origin, std::string symbol, std::string destination) {
    Transition transition(origin, symbol, destination);
    transitions.insert(transition);
}

bool DFA::containsState(std::string state) {
    return states.contains(state);
}

bool DFA::containsSymbol(std::string symbol) {
    return alphabet.contains(symbol);
}

bool DFA::containsFinalState(std::string state) {
    return finalStates.contains(state);
}

bool DFA::isValidSymbol(std::string symbol) {
    if (symbol.empty() || symbol == "-" || symbol == "ε" || symbol == "epsilon" || symbol == "λ" || symbol == "lambda") {
        return false;
    }

    for (int i = 0; i < (int)symbol.size(); i++) {
        char c = symbol[i];
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            return false;
        }
    }

    return true;
}

void DFA::checkNonEmptiness(LinkedList<std::string>& errors) {
    if (states.getSize() == 0) {
        errors.insert("The set of states is empty; a DFA must have at least one state");
    }
    if (alphabet.getSize() == 0) {
        errors.insert("The alphabet is empty; a DFA must have at least one symbol");
    }
}

void DFA::checkDuplicateStates(LinkedList<std::string>& errors) {
    for (int i = 0; i < states.getSize(); i++) {
        for (int j = i + 1; j < states.getSize(); j++) {
            if (states.get(i) == states.get(j)) {
                errors.insert("State '" + states.get(i) + "' is duplicated in the set of states");
            }
        }
    }
}

void DFA::checkDuplicateSymbols(LinkedList<std::string>& errors) {
    for (int i = 0; i < alphabet.getSize(); i++) {
        for (int j = i + 1; j < alphabet.getSize(); j++) {
            if (alphabet.get(i) == alphabet.get(j)) {
                errors.insert("Symbol '" + alphabet.get(i) + "' is duplicated in the alphabet");
            }
        }
    }
}

void DFA::checkSymbolValidity(LinkedList<std::string>& errors) {
    for (int i = 0; i < alphabet.getSize(); i++) {
        std::string symbol = alphabet.get(i);
        if (!isValidSymbol(symbol)) {
            errors.insert("Symbol '" + symbol + "' is not valid within the alphabet");
        }
    }
}

void DFA::checkInitialState(LinkedList<std::string>& errors) {
    if (initialState.empty()) {
        errors.insert("No initial state has been defined");
    } else if (!containsState(initialState)) {
        errors.insert("Initial state '" + initialState + "' does not belong to the set of states");
    }
}

void DFA::checkFinalStatesMembership(LinkedList<std::string>& errors) {
    for (int i = 0; i < finalStates.getSize(); i++) {
        std::string finalState = finalStates.get(i);
        if (!containsState(finalState)) {
            errors.insert("Final state '" + finalState + "' does not belong to the set of states");
        }
    }
}

void DFA::checkTransitionCompleteness(LinkedList<std::string>& errors) {
    for (int i = 0; i < states.getSize(); i++) {
        std::string state = states.get(i);
        for (int j = 0; j < alphabet.getSize(); j++) {
            std::string symbol = alphabet.get(j);
            int matches = 0;
            for (int k = 0; k < transitions.getSize(); k++) {
                Transition transition = transitions.get(k);
                if (transition.getOrigin() == state && transition.getSymbol() == symbol) {
                    matches++;
                }
            }

            if (matches == 0) {
                errors.insert("State '" + state + "' has no transition defined for symbol '" + symbol + "'");
            } else if (matches > 1) {
                errors.insert("State '" + state + "' has more than one transition for symbol '" + symbol + "' (non-determinism)");
            }
        }
    }
}

void DFA::checkTransitionDestinations(LinkedList<std::string>& errors) {
    for (int i = 0; i < transitions.getSize(); i++) {
        Transition transition = transitions.get(i);
        if (!containsState(transition.getDestination())) {
            errors.insert("Destination state '" + transition.getDestination() + "' is not registered in the set of states (origin '" + transition.getOrigin() + "', symbol '" + transition.getSymbol() + "')");
        }
    }
}

LinkedList<std::string> DFA::validate() {
    LinkedList<std::string> errors;

    checkNonEmptiness(errors);
    checkDuplicateStates(errors);
    checkDuplicateSymbols(errors);
    checkSymbolValidity(errors);
    checkInitialState(errors);
    checkFinalStatesMembership(errors);
    checkTransitionCompleteness(errors);
    checkTransitionDestinations(errors);

    return errors;
}

bool DFA::isValid() {
    return validate().getSize() == 0;
}

void DFA::printValidationReport() {
    LinkedList<std::string> errors = validate();

    if (errors.getSize() == 0) {
        std::cout << "The DFA is valid." << std::endl;
        return;
    }

    std::cout << "The DFA is NOT valid. Errors found:" << std::endl;
    for (int i = 0; i < errors.getSize(); i++) {
        std::cout << " - " << errors.get(i) << std::endl;
    }
}

void DFA::printDFA() {
    std::cout << "States: ";
    for (int i = 0; i < states.getSize(); i++) {
        std::cout << states.get(i) << " ";
    }
    std::cout << std::endl;

    std::cout << "Alphabet: ";
    for (int i = 0; i < alphabet.getSize(); i++) {
        std::cout << alphabet.get(i) << " ";
    }
    std::cout << std::endl;

    std::cout << "Initial State: " << initialState << std::endl;

    std::cout << "Final States: ";
    for (int i = 0; i < finalStates.getSize(); i++) {
        std::cout << finalStates.get(i) << " ";
    }
    std::cout << std::endl;

    std::cout << "Transitions: " << std::endl;
    for (int i = 0; i < transitions.getSize(); i++) {
        transitions.get(i).printTransition();
    }
}

std::string DFA::findTransitionDestination(std::string state, std::string symbol, bool& found) {
    for (int i = 0; i < transitions.getSize(); i++) {
        Transition transition = transitions.get(i);
        if (transition.getOrigin() == state && transition.getSymbol() == symbol) {
            found = true;
            return transition.getDestination();
        }
    }
    found = false;
    return "";
}

bool DFA::runString(std::string input, LinkedList<std::string>& trace) {
    std::string currentState = initialState;
    trace.insert("δ(" + currentState + ", ε) = " + currentState);

    for (int i = 0; i < (int)input.size(); i++) {
        std::string symbol(1, input[i]);
        bool found = false;
        std::string nextState = findTransitionDestination(currentState, symbol, found);

        if (!found) {
            trace.insert("δ(" + currentState + ", " + symbol + ") is not defined -> string rejected");
            return false;
        }

        trace.insert("δ(" + currentState + ", " + symbol + ") = " + nextState);
        currentState = nextState;
    }

    bool accepted = containsFinalState(currentState);
    trace.insert("Final state reached: '" + currentState + "' -> " + (accepted ? "is an accepting state" : "is not an accepting state"));
    return accepted;
}

DFA DFA::unionWith(DFA& other, LinkedList<std::string>& errors) {
    if (!isValid()) {
        errors.insert("The first DFA is not valid; validate it before computing the union");
    }
    if (!other.isValid()) {
        errors.insert("The second DFA is not valid; validate it before computing the union");
    }
    if (errors.getSize() > 0) {
        return DFA();
    }

    for (int i = 0; i < alphabet.getSize(); i++) {
        std::string symbol = alphabet.get(i);
        if (!other.containsSymbol(symbol)) {
            errors.insert("Symbol '" + symbol + "' is in the first alphabet but not in the second");
        }
    }
    for (int i = 0; i < other.alphabet.getSize(); i++) {
        std::string symbol = other.alphabet.get(i);
        if (!containsSymbol(symbol)) {
            errors.insert("Symbol '" + symbol + "' is in the second alphabet but not in the first");
        }
    }

    DFA result;
    if (errors.getSize() > 0) {
        return result;
    }

    for (int i = 0; i < alphabet.getSize(); i++) {
        result.addSymbol(alphabet.get(i));
    }

    for (int i = 0; i < states.getSize(); i++) {
        std::string stateA = states.get(i);
        for (int j = 0; j < other.states.getSize(); j++) {
            std::string stateB = other.states.get(j);
            std::string composite = stateA + "," + stateB;
            result.addState(composite);

            if (containsFinalState(stateA) || other.containsFinalState(stateB)) {
                result.addFinalState(composite);
            }
        }
    }

    result.setInitialState(initialState + "," + other.initialState);

    for (int i = 0; i < states.getSize(); i++) {
        std::string stateA = states.get(i);
        for (int j = 0; j < other.states.getSize(); j++) {
            std::string stateB = other.states.get(j);
            std::string composite = stateA + "," + stateB;

            for (int k = 0; k < alphabet.getSize(); k++) {
                std::string symbol = alphabet.get(k);
                bool foundA = false;
                bool foundB = false;
                std::string destA = findTransitionDestination(stateA, symbol, foundA);
                std::string destB = other.findTransitionDestination(stateB, symbol, foundB);

                if (foundA && foundB) {
                    result.addTransition(composite, symbol, destA + "," + destB);
                }
            }
        }
    }

    return result;
}

//Vamos a dormirnos ya, ya es tarde gente.
//git add git commit git push y a la cama
//25/08/2026

//se continua a las 9am
