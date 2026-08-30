#include <iostream>
#include <string>

class Transition {

    std::string origin;
    std::string symbol;
    std::string destination;

public:
    Transition(std::string origin, std::string symbol, std::string destination);

    void printTransition();

    std::string getOrigin();

    std::string getSymbol();

    std::string getDestination();


};
