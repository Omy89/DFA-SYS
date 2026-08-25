#include <iostream>
#include <string>

struct Node {
    std::string data;
    Node* next;

    Node(std::string data){
        this->data = data;
        this->next = nullptr;
    }
};

class LinkedList {

private:
    Node* head;
    int size;

public:


    LinkedList(){
        this->head = nullptr;
        this->size = 0;
    };

    void insert(std::string data){
        Node* newNode = new Node(data);
        if (head == nullptr) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
        size++;
    }

    void show() {
        Node* current = head;
        while (current != nullptr) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }


    ~LinkedList(){
        Node* current = head;
        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    bool contains(std::string data) {
        Node* current = head;
        while (current != nullptr) {
            if (current->data == data) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    std::string get(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }
        Node* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        return current->data;
    }

    int getSize() {
        return size;
    }

};