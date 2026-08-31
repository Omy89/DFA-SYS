#pragma once
#include <QMainWindow>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>

#include "DFA.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

    //varios DFAs en memoria (no se guardan a disco): nombre y automata van paralelos por indice, como states/alphabet en DFA
    LinkedList<std::string> dfaNames;
    LinkedList<DFA> dfas;
    int currentIndex;

    QComboBox* dfaSelector;
    QPushButton* newDfaBtn;

    QLineEdit* stateInput;
    QLineEdit* symbolInput;
    QLineEdit* initialStateInput;
    QLineEdit* finalStateInput;
    QLineEdit* originInput;
    QLineEdit* transitionSymbolInput;
    QLineEdit* destinationInput;

    QTableWidget* deltaTable;
    QPlainTextEdit* deltaList;

    QLineEdit* testStringInput;
    QPushButton* testStringBtn;

    QPlainTextEdit* log;

public:
    MainWindow(QWidget* parent = nullptr);

private slots:
    void onNewDfa();

    void onSelectDfa(int index);

    void onAddState();

    void onAddSymbol();

    void onSetInitialState();

    void onAddFinalState();

    void onAddTransition();

    void onValidate();

    void onTestString();

private:
    DFA& currentDfa();

    //printDFA()/printValidationReport() escriben a std::cout, asi que lo redirigimos a un string para mostrarlo en la ventana
    void refreshLog();

    //redibuja la tabla delta (estados x alfabeto) y la lista en notacion de flecha "origen-simbolo->destino"
    void refreshDeltaView();

    void refreshAll();
};
