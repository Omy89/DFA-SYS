#include <iostream>
#include <stdexcept> 

template <typename T>

struct Node {
    T data;
    Node* next;

    //data must be set via the initializer list (not assigned in the body), otherwise
    //the compiler default-constructs "data" first, which breaks for any T with no default constructor
    Node(T data) : data(data), next(nullptr) {}
};


//dios mio ya dias no hago estructuras de datos, que nostalgia, que recuerdos
//copilot ya me esta agarrando mi forma de comentar
//hola ingeniero
//inge quiero pasar la clase
//no se si vaya a leer el codigo, pero si lo hace, que sepa que lo quiero mucho y que es un gran ingeniero
//eso de arriba lo escribio copilot
//que risa
//estoy hablando solo a las 2 de la mañana


template <typename T>
class LinkedList {

private:
    Node<T>* head;
    int size;

public:

    LinkedList(){
        this->head = nullptr;
        this->size = 0;
    };

    //para hacer una copia profunda de la lista enlazada
    LinkedList(const LinkedList<T>& other) {
        head = nullptr;
        size = 0;
        Node<T>* current = other.head;
        while (current != nullptr) {
            insert(current->data);
            current = current->next;
        }
    }

    void insert(T data){
        Node<T>* newNode = new Node<T>(data);
        if (head == nullptr) {
            head = newNode;
        } else {
            Node<T>* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
        size++;
    }

    ~LinkedList(){
        Node<T>* current = head;
        while (current != nullptr) {
            Node<T>* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    bool contains(T data) {
        Node<T>* current = head;
        while (current != nullptr) {        
            if (current->data == data) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    T get(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }
        Node<T>* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        return current->data;
    }

    int getSize() {
        return size;
    }

};