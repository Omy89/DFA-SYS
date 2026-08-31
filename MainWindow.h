#pragma once
#include <QMainWindow>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QTabWidget>

#include "DFA.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

    LinkedList<std::string> dfaNames;
    LinkedList<DFA> dfas;
    int currentIndex;

    LinkedList<int> unionSourceA;
    LinkedList<int> unionSourceB;

    QComboBox* dfaSelector;
    QPushButton* newDfaBtn;

    QComboBox* unionASelector;
    QComboBox* unionBSelector;
    QPushButton* computeUnionBtn;

    QLineEdit* stateInput;
    QLineEdit* symbolInput;
    QLineEdit* initialStateInput;
    QLineEdit* finalStateInput;
    QLineEdit* originInput;
    QLineEdit* transitionSymbolInput;
    QLineEdit* destinationInput;

    QTableWidget* deltaTable;
    QPlainTextEdit* deltaList;

    QPlainTextEdit* buildLog;

    QLineEdit* testStringInput;
    QPushButton* testStringBtn;
    QPlainTextEdit* testLog;

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

    void onComputeUnion();

    void onValidate();

    void onTestString();

private:
    QWidget* buildCreatePage();

    QWidget* buildTestPage();

    DFA& currentDfa();

    void refreshBuildLog();

    void refreshDeltaView();

    void refreshAll();
};
