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

//al inicio lo habia hecho en consola, pero pues por el cambio necesite esto.
static std::string captureCout(std::function<void()> fn) {
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    fn();
    std::cout.rdbuf(old);
    return buffer.str();
}

static QWidget* wrapRow(QLineEdit* edit, QPushButton* button) {
    QWidget* row = new QWidget();
    QHBoxLayout* rowLayout = new QHBoxLayout(row);
    rowLayout->setContentsMargins(0, 0, 0, 0);
    rowLayout->addWidget(edit);
    rowLayout->addWidget(button);
    return row;
}

static QWidget* wrapScroll(QWidget* content) {
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidget(content);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    return scrollArea;
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), currentIndex(-1) {
    resize(880, 800);

    QWidget* central = new QWidget();
    QVBoxLayout* rootLayout = new QVBoxLayout(central);

    QHBoxLayout* selectorRow = new QHBoxLayout();
    dfaSelector = new QComboBox();
    newDfaBtn = new QPushButton("+ New DFA");
    selectorRow->addWidget(new QLabel("DFA:"));
    selectorRow->addWidget(dfaSelector, 1);
    selectorRow->addWidget(newDfaBtn);
    rootLayout->addLayout(selectorRow);

    QTabWidget* tabs = new QTabWidget();
    tabs->addTab(buildCreatePage(), "Create DFA");
    tabs->addTab(buildTestPage(), "Test DFA");
    rootLayout->addWidget(tabs, 1);

    setCentralWidget(central);

    connect(newDfaBtn, &QPushButton::clicked, this, &MainWindow::onNewDfa);
    connect(dfaSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onSelectDfa);

    dfaNames.insert("DFA 1");
    dfas.insert(DFA());
    unionSourceA.insert(-1);
    unionSourceB.insert(-1);
    dfaSelector->addItem("DFA 1");
    unionASelector->addItem("DFA 1");
    unionBSelector->addItem("DFA 1");
    dfaSelector->setCurrentIndex(0);
    onSelectDfa(0);
}

QWidget* MainWindow::buildCreatePage() {
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);

    QGroupBox* unionGroup = new QGroupBox(QString::fromUtf8("Union (∪)"));
    QHBoxLayout* unionLayout = new QHBoxLayout();
    unionASelector = new QComboBox();
    unionBSelector = new QComboBox();
    computeUnionBtn = new QPushButton(QString::fromUtf8("Compute A ∪ B"));
    QLabel* unionOpLabel = new QLabel(QString::fromUtf8("∪"));
    unionOpLabel->setAlignment(Qt::AlignCenter);
    unionLayout->addWidget(unionASelector, 1);
    unionLayout->addWidget(unionOpLabel);
    unionLayout->addWidget(unionBSelector, 1);
    unionLayout->addWidget(computeUnionBtn);
    unionGroup->setLayout(unionLayout);
    layout->addWidget(unionGroup);

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
    layout->addWidget(buildGroup);

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
    layout->addWidget(deltaGroup);

    QPushButton* validateBtn = new QPushButton("Validate DFA");
    layout->addWidget(validateBtn);

    buildLog = new QPlainTextEdit();
    buildLog->setReadOnly(true);
    layout->addWidget(buildLog, 1);

    connect(addStateBtn, &QPushButton::clicked, this, &MainWindow::onAddState);
    connect(addSymbolBtn, &QPushButton::clicked, this, &MainWindow::onAddSymbol);
    connect(setInitialBtn, &QPushButton::clicked, this, &MainWindow::onSetInitialState);
    connect(addFinalBtn, &QPushButton::clicked, this, &MainWindow::onAddFinalState);
    connect(addTransitionBtn, &QPushButton::clicked, this, &MainWindow::onAddTransition);
    connect(computeUnionBtn, &QPushButton::clicked, this, &MainWindow::onComputeUnion);
    connect(validateBtn, &QPushButton::clicked, this, &MainWindow::onValidate);

    return wrapScroll(page);
}

QWidget* MainWindow::buildTestPage() {
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);

    QGroupBox* testGroup = new QGroupBox("Test String");
    QHBoxLayout* testLayout = new QHBoxLayout();
    testStringInput = new QLineEdit();
    testStringInput->setPlaceholderText("aab");
    testStringBtn = new QPushButton("Test String");
    testLayout->addWidget(testStringInput);
    testLayout->addWidget(testStringBtn);
    testGroup->setLayout(testLayout);
    layout->addWidget(testGroup);

    testLog = new QPlainTextEdit();
    testLog->setReadOnly(true);
    layout->addWidget(testLog, 1);

    connect(testStringBtn, &QPushButton::clicked, this, &MainWindow::onTestString);

    return wrapScroll(page);
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
    unionSourceA.insert(-1);
    unionSourceB.insert(-1);
    dfaSelector->addItem(QString::fromStdString(stdName));
    unionASelector->addItem(QString::fromStdString(stdName));
    unionBSelector->addItem(QString::fromStdString(stdName));
    dfaSelector->setCurrentIndex(dfaSelector->count() - 1);
}

void MainWindow::onComputeUnion() {
    int indexA = unionASelector->currentIndex();
    int indexB = unionBSelector->currentIndex();
    if (indexA < 0 || indexB < 0) {
        return;
    }

    LinkedList<std::string> errors;
    DFA result = dfas.get(indexA).unionWith(dfas.get(indexB), errors);

    if (errors.getSize() > 0) {
        QString msg;
        for (int i = 0; i < errors.getSize(); i++) {
            msg += QString::fromStdString(errors.get(i)) + "\n";
        }
        QMessageBox::warning(this, "Union", msg);
        return;
    }

    std::string name = dfaSelector->itemText(indexA).toStdString() + " U " + dfaSelector->itemText(indexB).toStdString();
    std::string base = name;
    int suffix = 2;
    while (dfaNames.contains(name)) {
        name = base + " (" + std::to_string(suffix) + ")";
        suffix++;
    }

    dfaNames.insert(name);
    dfas.insert(result);
    unionSourceA.insert(indexA);
    unionSourceB.insert(indexB);

    QString qname = QString::fromStdString(name);
    dfaSelector->addItem(qname);
    unionASelector->addItem(qname);
    unionBSelector->addItem(qname);
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
    buildLog->appendPlainText(QString::fromStdString(report));
}

void MainWindow::onTestString() {
    if (!currentDfa().isValid()) {
        QMessageBox::warning(this, "Test String", "This DFA is not valid. Validate it first on the Create DFA tab.");
        return;
    }

    std::string input = testStringInput->text().toStdString();
    LinkedList<std::string> trace;
    bool accepted = currentDfa().runString(input, trace);

    QString block = QString("--- Testing '%1' on '%2' ---\n").arg(QString::fromStdString(input), dfaSelector->currentText());
    for (int i = 0; i < trace.getSize(); i++) {
        block += QString::fromStdString(trace.get(i)) + "\n";
    }
    block += QString("Result: %1\n").arg(accepted ? "ACCEPTED" : "REJECTED");

    int sourceA = unionSourceA.get(currentIndex);
    int sourceB = unionSourceB.get(currentIndex);
    if (sourceA >= 0 && sourceB >= 0) {
        LinkedList<std::string> traceA;
        LinkedList<std::string> traceB;
        bool acceptedA = dfas.get(sourceA).runString(input, traceA);
        bool acceptedB = dfas.get(sourceB).runString(input, traceB);
        block += QString("%1: %2\n").arg(dfaSelector->itemText(sourceA), acceptedA ? "ACCEPTED" : "REJECTED");
        block += QString("%1: %2\n").arg(dfaSelector->itemText(sourceB), acceptedB ? "ACCEPTED" : "REJECTED");
        block += QString("Union: %1\n").arg(accepted ? "ACCEPTED" : "REJECTED");
    }

    testLog->appendPlainText(block);
}

void MainWindow::refreshBuildLog() {
    std::string summary = captureCout([this]() { currentDfa().printDFA(); });
    buildLog->setPlainText(QString::fromStdString(summary));
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

        std::string label = state;
        if (state == initial) {
            label = "->" + label;
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

    QString arrowLines;
    for (int k = 0; k < transitions.getSize(); k++) {
        Transition transition = transitions.get(k);
        arrowLines += QString::fromStdString(transition.getOrigin() + "-" + transition.getSymbol() + "->" + transition.getDestination()) + "\n";
    }
    deltaList->setPlainText(arrowLines);
}

void MainWindow::refreshAll() {
    refreshBuildLog();
    refreshDeltaView();
}
