#include "MainWindow.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QScrollArea>
#include <QInputDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <sstream>
#include <functional>

//metodo libre chiquito para capturar lo que printDFA()/printValidationReport() mandan a std::cout
static std::string captureCout(std::function<void()> fn) {
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    fn();
    std::cout.rdbuf(old);
    return buffer.str();
}

//junta un QLineEdit y un QPushButton en una sola fila, para no repetir el mismo layout una y otra vez
static QWidget* wrapRow(QLineEdit* edit, QPushButton* button) {
    QWidget* row = new QWidget();
    QHBoxLayout* rowLayout = new QHBoxLayout(row);
    rowLayout->setContentsMargins(0, 0, 0, 0);
    rowLayout->addWidget(edit);
    rowLayout->addWidget(button);
    return row;
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), currentIndex(-1) {
    resize(880, 800);

    QWidget* container = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(container);

    //--- selector de DFA: se pueden tener varios en memoria a la vez ---
    QHBoxLayout* selectorRow = new QHBoxLayout();
    dfaSelector = new QComboBox();
    newDfaBtn = new QPushButton("+ New DFA");
    selectorRow->addWidget(new QLabel("DFA:"));
    selectorRow->addWidget(dfaSelector, 1);
    selectorRow->addWidget(newDfaBtn);
    mainLayout->addLayout(selectorRow);

    //--- formulario para armar la quintupla ---
    QGroupBox* buildGroup = new QGroupBox("Build DFA");
    QFormLayout* buildLayout = new QFormLayout();

    stateInput = new QLineEdit();
    stateInput->setPlaceholderText("q0");
    QPushButton* addStateBtn = new QPushButton("Add State");
    buildLayout->addRow("State:", wrapRow(stateInput, addStateBtn));

    symbolInput = new QLineEdit();
    symbolInput->setPlaceholderText("a");
    QPushButton* addSymbolBtn = new QPushButton("Add Symbol");
    buildLayout->addRow("Symbol:", wrapRow(symbolInput, addSymbolBtn));

    initialStateInput = new QLineEdit();
    initialStateInput->setPlaceholderText("q0");
    QPushButton* setInitialBtn = new QPushButton("Set Initial");
    buildLayout->addRow("Initial State:", wrapRow(initialStateInput, setInitialBtn));

    finalStateInput = new QLineEdit();
    finalStateInput->setPlaceholderText("q1");
    QPushButton* addFinalBtn = new QPushButton("Add Final State");
    buildLayout->addRow("Final State:", wrapRow(finalStateInput, addFinalBtn));

    //fila de transicion en notacion delta: origen -simbolo-> destino
    QWidget* transitionRow = new QWidget();
    QHBoxLayout* transitionLayout = new QHBoxLayout(transitionRow);
    transitionLayout->setContentsMargins(0, 0, 0, 0);
    originInput = new QLineEdit();
    originInput->setPlaceholderText("q0");
    transitionSymbolInput = new QLineEdit();
    transitionSymbolInput->setPlaceholderText("a");
    destinationInput = new QLineEdit();
    destinationInput->setPlaceholderText("q1");
    QPushButton* addTransitionBtn = new QPushButton("Add Transition");
    transitionLayout->addWidget(originInput);
    transitionLayout->addWidget(new QLabel("-"));
    transitionLayout->addWidget(transitionSymbolInput);
    transitionLayout->addWidget(new QLabel("->"));
    transitionLayout->addWidget(destinationInput);
    transitionLayout->addWidget(addTransitionBtn);
    buildLayout->addRow("Transition:", transitionRow);

    buildGroup->setLayout(buildLayout);
    mainLayout->addWidget(buildGroup);

    //--- delta: tabla estados x alfabeto, y la misma info en notacion de flecha ---
    QGroupBox* deltaGroup = new QGroupBox(QString::fromUtf8("δ (Transition Function)"));
    QVBoxLayout* deltaLayout = new QVBoxLayout();

    deltaTable = new QTableWidget();
    deltaTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    deltaTable->setMinimumHeight(160);
    deltaLayout->addWidget(deltaTable);

    deltaList = new QPlainTextEdit();
    deltaList->setReadOnly(true);
    deltaList->setMaximumHeight(120);
    deltaList->setPlaceholderText("q0-a->q1");
    deltaLayout->addWidget(deltaList);

    deltaGroup->setLayout(deltaLayout);
    mainLayout->addWidget(deltaGroup);

    //--- probar una cadena contra el DFA seleccionado ---
    QGroupBox* testGroup = new QGroupBox("Test String (Cadena)");
    QHBoxLayout* testLayout = new QHBoxLayout();
    testStringInput = new QLineEdit();
    testStringInput->setPlaceholderText("aab");
    testStringBtn = new QPushButton("Test String");
    testLayout->addWidget(testStringInput);
    testLayout->addWidget(testStringBtn);
    testGroup->setLayout(testLayout);
    mainLayout->addWidget(testGroup);

    QPushButton* validateBtn = new QPushButton("Validate DFA");
    mainLayout->addWidget(validateBtn);

    log = new QPlainTextEdit();
    log->setReadOnly(true);
    mainLayout->addWidget(log, 1);

    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidget(container);
    scrollArea->setWidgetResizable(true);
    setCentralWidget(scrollArea);

    connect(newDfaBtn, &QPushButton::clicked, this, &MainWindow::onNewDfa);
    connect(dfaSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onSelectDfa);
    connect(addStateBtn, &QPushButton::clicked, this, &MainWindow::onAddState);
    connect(addSymbolBtn, &QPushButton::clicked, this, &MainWindow::onAddSymbol);
    connect(setInitialBtn, &QPushButton::clicked, this, &MainWindow::onSetInitialState);
    connect(addFinalBtn, &QPushButton::clicked, this, &MainWindow::onAddFinalState);
    connect(addTransitionBtn, &QPushButton::clicked, this, &MainWindow::onAddTransition);
    connect(validateBtn, &QPushButton::clicked, this, &MainWindow::onValidate);
    connect(testStringBtn, &QPushButton::clicked, this, &MainWindow::onTestString);

    //arranca con un DFA en blanco para no obligar a crear uno antes de poder usar la ventana
    dfaNames.insert("DFA 1");
    dfas.insert(DFA());
    dfaSelector->addItem("DFA 1");
    dfaSelector->setCurrentIndex(0);
    onSelectDfa(0);
}

DFA& MainWindow::currentDfa() {
    return dfas.get(currentIndex);
}

void MainWindow::onNewDfa() {
    bool ok = false;
    QString name = QInputDialog::getText(this, "New DFA", "Name:", QLineEdit::Normal, "", &ok);
    if (!ok) {
        return;
    }

    std::string stdName = name.trimmed().toStdString();
    if (stdName.empty()) {
        return;
    }
    if (dfaNames.contains(stdName)) {
        QMessageBox::warning(this, "New DFA", "That name is already in use.");
        return;
    }

    dfaNames.insert(stdName);
    dfas.insert(DFA());
    dfaSelector->addItem(QString::fromStdString(stdName));
    dfaSelector->setCurrentIndex(dfaSelector->count() - 1);
}

void MainWindow::onSelectDfa(int index) {
    if (index < 0 || index >= dfas.getSize()) {
        return;
    }
    currentIndex = index;
    setWindowTitle("DFA-SYS - " + dfaSelector->currentText());
    refreshAll();
}

void MainWindow::onAddState() {
    if (!stateInput->text().isEmpty()) {
        currentDfa().addState(stateInput->text().toStdString());
        stateInput->clear();
        refreshAll();
    }
}

void MainWindow::onAddSymbol() {
    if (!symbolInput->text().isEmpty()) {
        currentDfa().addSymbol(symbolInput->text().toStdString());
        symbolInput->clear();
        refreshAll();
    }
}

void MainWindow::onSetInitialState() {
    if (!initialStateInput->text().isEmpty()) {
        currentDfa().setInitialState(initialStateInput->text().toStdString());
        initialStateInput->clear();
        refreshAll();
    }
}

void MainWindow::onAddFinalState() {
    if (!finalStateInput->text().isEmpty()) {
        currentDfa().addFinalState(finalStateInput->text().toStdString());
        finalStateInput->clear();
        refreshAll();
    }
}

void MainWindow::onAddTransition() {
    if (!originInput->text().isEmpty() && !transitionSymbolInput->text().isEmpty() && !destinationInput->text().isEmpty()) {
        currentDfa().addTransition(originInput->text().toStdString(), transitionSymbolInput->text().toStdString(), destinationInput->text().toStdString());
        originInput->clear();
        transitionSymbolInput->clear();
        destinationInput->clear();
        refreshAll();
    }
}

void MainWindow::onValidate() {
    std::string report = captureCout([this]() { currentDfa().printValidationReport(); });
    log->appendPlainText(QString::fromStdString(report));
}

void MainWindow::onTestString() {
    std::string input = testStringInput->text().toStdString();
    LinkedList<std::string> trace;
    bool accepted = currentDfa().runString(input, trace);

    QString block = QString("--- Testing '%1' on '%2' ---\n").arg(QString::fromStdString(input), dfaSelector->currentText());
    for (int i = 0; i < trace.getSize(); i++) {
        block += QString::fromStdString(trace.get(i)) + "\n";
    }
    block += QString("Result: %1\n").arg(accepted ? "ACCEPTED" : "REJECTED");

    log->appendPlainText(block);
}

void MainWindow::refreshLog() {
    std::string summary = captureCout([this]() { currentDfa().printDFA(); });
    log->setPlainText(QString::fromStdString(summary));
}

void MainWindow::refreshDeltaView() {
    DFA& dfa = currentDfa();
    LinkedList<std::string> states = dfa.getStates();
    LinkedList<std::string> alphabet = dfa.getAlphabet();
    LinkedList<std::string> finalStates = dfa.getFinalStates();
    LinkedList<Transition> transitions = dfa.getTransitions();
    std::string initial = dfa.getInitialState();

    deltaTable->clear();
    deltaTable->setRowCount(states.getSize());
    deltaTable->setColumnCount(alphabet.getSize());

    QStringList columnHeaders;
    for (int j = 0; j < alphabet.getSize(); j++) {
        columnHeaders << QString::fromStdString(alphabet.get(j));
    }
    deltaTable->setHorizontalHeaderLabels(columnHeaders);

    QStringList rowHeaders;
    for (int i = 0; i < states.getSize(); i++) {
        std::string state = states.get(i);

        //flecha para el estado inicial, asterisco para los estados de aceptacion, igual que pide la especificacion
        std::string label = state;
        if (state == initial) {
            label = QString::fromUtf8("→ ").toStdString() + label;
        }
        if (finalStates.contains(state)) {
            label = label + " *";
        }
        rowHeaders << QString::fromStdString(label);

        for (int j = 0; j < alphabet.getSize(); j++) {
            std::string symbol = alphabet.get(j);
            std::string destination = "-";
            for (int k = 0; k < transitions.getSize(); k++) {
                Transition transition = transitions.get(k);
                if (transition.getOrigin() == state && transition.getSymbol() == symbol) {
                    destination = transition.getDestination();
                    break;
                }
            }
            QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(destination));
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            deltaTable->setItem(i, j, item);
        }
    }
    deltaTable->setVerticalHeaderLabels(rowHeaders);
    deltaTable->resizeColumnsToContents();

    //la misma delta pero en notacion compacta de flecha: q0-a->q1
    QString arrowLines;
    for (int k = 0; k < transitions.getSize(); k++) {
        Transition transition = transitions.get(k);
        arrowLines += QString::fromStdString(transition.getOrigin() + "-" + transition.getSymbol() + "->" + transition.getDestination()) + "\n";
    }
    deltaList->setPlainText(arrowLines);
}

void MainWindow::refreshAll() {
    refreshLog();
    refreshDeltaView();
}
