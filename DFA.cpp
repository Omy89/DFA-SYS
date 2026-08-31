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

//simbolos reservados/nulos que el enunciado prohibe en el alfabeto: epsilon, lambda, vacio, espacios y guion
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

//unicidad de estados: comparacion de cada par con busqueda propia
void DFA::checkDuplicateStates(LinkedList<std::string>& errors) {
    for (int i = 0; i < states.getSize(); i++) {
        for (int j = i + 1; j < states.getSize(); j++) {
            if (states.get(i) == states.get(j)) {
                errors.insert("El estado '" + states.get(i) + "' esta duplicado en el conjunto de estados");
            }
        }
    }
}

//unicidad del alfabeto
void DFA::checkDuplicateSymbols(LinkedList<std::string>& errors) {
    for (int i = 0; i < alphabet.getSize(); i++) {
        for (int j = i + 1; j < alphabet.getSize(); j++) {
            if (alphabet.get(i) == alphabet.get(j)) {
                errors.insert("El simbolo '" + alphabet.get(i) + "' esta duplicado en el alfabeto");
            }
        }
    }
}

//simbologia valida del alfabeto
void DFA::checkSymbolValidity(LinkedList<std::string>& errors) {
    for (int i = 0; i < alphabet.getSize(); i++) {
        std::string symbol = alphabet.get(i);
        if (!isValidSymbol(symbol)) {
            errors.insert("El simbolo '" + symbol + "' no es valido dentro del alfabeto");
        }
    }
}

//debe existir exactamente un estado inicial y pertenecer al conjunto de estados
void DFA::checkInitialState(LinkedList<std::string>& errors) {
    if (initialState.empty()) {
        errors.insert("No se ha definido un estado inicial");
    } else if (!containsState(initialState)) {
        errors.insert("El estado inicial '" + initialState + "' no pertenece al conjunto de estados");
    }
}

//todo estado final debe pertenecer al conjunto de estados (la lista vacia es valida)
void DFA::checkFinalStatesMembership(LinkedList<std::string>& errors) {
    for (int i = 0; i < finalStates.getSize(); i++) {
        std::string finalState = finalStates.get(i);
        if (!containsState(finalState)) {
            errors.insert("El estado final '" + finalState + "' no pertenece al conjunto de estados");
        }
    }
}

//completitud y determinismo: cada par (estado, simbolo) debe tener exactamente una transicion
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
                errors.insert("El estado '" + state + "' carece de transicion para el simbolo '" + symbol + "'");
            } else if (matches > 1) {
                errors.insert("El estado '" + state + "' tiene mas de una transicion para el simbolo '" + symbol + "' (no-determinismo)");
            }
        }
    }
}

//integridad de destino: todo destino de una transicion debe ser un estado registrado
void DFA::checkTransitionDestinations(LinkedList<std::string>& errors) {
    for (int i = 0; i < transitions.getSize(); i++) {
        Transition transition = transitions.get(i);
        if (!containsState(transition.getDestination())) {
            errors.insert("El estado de destino '" + transition.getDestination() + "' no esta registrado en el conjunto de estados (origen '" + transition.getOrigin() + "', simbolo '" + transition.getSymbol() + "')");
        }
    }
}

//recorre las estructuras manuales y arma el informe de errores del DFA, un chequeo por funcion
LinkedList<std::string> DFA::validate() {
    LinkedList<std::string> errors;

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
        std::cout << "El DFA es valido." << std::endl;
        return;
    }

    std::cout << "El DFA NO es valido. Errores encontrados:" << std::endl;
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

//busqueda propia sobre transitions, sin usar find()/algoritmos nativos: recorre y compara origen+simbolo
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

//delta*(q0, cadena): arranca en delta(q0, epsilon) = q0 (todavia no se consumio nada) y despues aplica delta simbolo por simbolo.
//se asume que cada simbolo del alfabeto es un solo caracter, que es el caso normal para probar cadenas en un DFA.
bool DFA::runString(std::string input, LinkedList<std::string>& trace) {
    std::string currentState = initialState;
    trace.insert("δ(" + currentState + ", ε) = " + currentState);

    for (int i = 0; i < (int)input.size(); i++) {
        std::string symbol(1, input[i]);
        bool found = false;
        std::string nextState = findTransitionDestination(currentState, symbol, found);

        if (!found) {
            trace.insert("δ(" + currentState + ", " + symbol + ") no esta definida -> cadena rechazada");
            return false;
        }

        trace.insert("δ(" + currentState + ", " + symbol + ") = " + nextState);
        currentState = nextState;
    }

    bool accepted = containsFinalState(currentState);
    trace.insert("Estado final alcanzado: '" + currentState + "' -> " + (accepted ? "es de aceptacion" : "no es de aceptacion"));
    return accepted;
}

//Vamos a dormirnos ya, ya es tarde gente.
//git add git commit git push y a la cama
//25/08/2026

//se continua a las 9am
