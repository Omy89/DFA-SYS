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

    void printDFA();

    LinkedList<std::string> getStates(){

    }

    

};
