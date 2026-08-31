#pragma once
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

    DFA();

    void addState(std::string state);

    void addSymbol(std::string symbol);

    void setInitialState(std::string state);

    void addFinalState(std::string state);

    void addTransition(std::string origin, std::string symbol, std::string destination);

    bool containsState(std::string state);

    bool containsSymbol(std::string symbol);

    bool containsFinalState(std::string state);

    bool isValidSymbol(std::string symbol);

    LinkedList<std::string> validate();

    bool isValid();

    void printValidationReport();

    void printDFA();

    LinkedList<std::string> getStates(){
        return states;
    }

    LinkedList<std::string> getAlphabet(){
        return alphabet;
    }

    LinkedList<std::string> getFinalStates(){
        return finalStates;
    }

    LinkedList<Transition> getTransitions(){
        return transitions;
    }

    std::string getInitialState(){
        return initialState;
    }

    bool runString(std::string input, LinkedList<std::string>& trace);

    DFA unionWith(DFA& other, LinkedList<std::string>& errors);

private:

    void checkNonEmptiness(LinkedList<std::string>& errors);

    void checkDuplicateStates(LinkedList<std::string>& errors);

    void checkDuplicateSymbols(LinkedList<std::string>& errors);

    void checkSymbolValidity(LinkedList<std::string>& errors);

    void checkInitialState(LinkedList<std::string>& errors);

    void checkFinalStatesMembership(LinkedList<std::string>& errors);

    void checkTransitionCompleteness(LinkedList<std::string>& errors);

    void checkTransitionDestinations(LinkedList<std::string>& errors);

    std::string findTransitionDestination(std::string state, std::string symbol, bool& found);

};
