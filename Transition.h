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
};