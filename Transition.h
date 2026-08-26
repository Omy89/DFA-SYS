#include <iostream>
#include <string>

class Transition {

    std::string origin;
    std::string symbol;
    std::string destination;

public:
    Transition(std::string origin, std::string symbol, std::string destination) {
        this->origin = origin;
        this->symbol = symbol;
        this->destination = destination;
    }

    void printTransition() {
        std::cout << "Origin: " << origin << ", Symbol: " << symbol << ", Destination: " << destination << std::endl;
    }

    std::string getOrigin() {
        return origin;
    }

    std::string getSymbol() {
        return symbol;
    }

    std::string getDestination() {
        return destination;
    }


};