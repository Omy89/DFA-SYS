#include "Transition.h"

Transition::Transition(std::string origin, std::string symbol, std::string destination) {
    this->origin = origin;
    this->symbol = symbol;
    this->destination = destination;
}

void Transition::printTransition() {
    std::cout << "Origin: " << origin << ", Symbol: " << symbol << ", Destination: " << destination << std::endl;
}

std::string Transition::getOrigin() {
    return origin;
}

std::string Transition::getSymbol() {
    return symbol;
}

std::string Transition::getDestination() {
    return destination;
}
