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

    //Modulo 1: validacion estricta del DFA (unicidad, alfabeto, estado inicial/finales, delta total y determinista)
    bool isValidSymbol(std::string symbol);

    LinkedList<std::string> validate();

    bool isValid();

    void printValidationReport();

    void printDFA();

    //getters de solo lectura para que la UI pueda dibujar la tabla/lista de delta sin tocar los datos privados
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

    //funcion de transicion extendida delta*: arranca en delta(q0, epsilon) = q0 y consume la cadena simbolo por simbolo.
    //llena "trace" con cada paso delta(estado, simbolo) = destino y devuelve si la cadena termino en un estado de aceptacion
    bool runString(std::string input, LinkedList<std::string>& trace);

private:

    //cada chequeo de validate() vive en su propia funcion y le agrega sus errores a la lista que le pasan por referencia
    void checkDuplicateStates(LinkedList<std::string>& errors);

    void checkDuplicateSymbols(LinkedList<std::string>& errors);

    void checkSymbolValidity(LinkedList<std::string>& errors);

    void checkInitialState(LinkedList<std::string>& errors);

    void checkFinalStatesMembership(LinkedList<std::string>& errors);

    void checkTransitionCompleteness(LinkedList<std::string>& errors);

    void checkTransitionDestinations(LinkedList<std::string>& errors);

    //busqueda propia de la transicion definida para (state, symbol); found queda en false si delta no esta definida ahi
    std::string findTransitionDestination(std::string state, std::string symbol, bool& found);

};
