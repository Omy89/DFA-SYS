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



private:

void addState(std::string state) {
    states.insert(state);
}



//Vamos a dormirnos ya, ya es tarde gente.
//git add git commit git push y a la cama
//25/08/2026
};