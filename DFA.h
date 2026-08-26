#include <iostream>
#include <string>

#include "LinkedList.h"
#include "Transition.h"

class DFA {
    //vamo a definir la quintupla
    LinkedList<std::string> states;
    LinkedList<std::string> alphabet;
    std::string initialState;
    LinkedList<std::string> finalStates;
    LinkedList<Transition> transitions;


public:

    DFA() {
        initialState = "";
    }
    
void addState(std::string state) {
    states.insert(state);
}

void addSymbol(std::string symbol) {
    alphabet.insert(symbol);
}

void setInitialState(std::string state) {
    initialState = state;
}

void addFinalState(std::string state) {
    finalStates.insert(state);
}

void addTransition(std::string origin, std::string symbol, std::string destination) {
    Transition transition(origin, symbol, destination);
    transitions.insert(transition);
}

void printDFA() {
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



//Vamos a dormirnos ya, ya es tarde gente.
//git add git commit git push y a la cama
//25/08/2026

//se continua a las 9am
};