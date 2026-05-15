#ifndef STACK_H
#define STACK_H

#include <iostream>

template<typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        
        Node(T val) : data(val), next(nullptr) {}
    };
    
    Node* topNode;
    int size;

public:
    Stack() : topNode(nullptr), size(0) {}

    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }

    // Push element onto stack
    void push(T value) {
        Node* newNode = new Node(value);
        newNode->next = topNode;
        topNode = newNode;
        size++;
    }

    // Pop element from stack
    T pop() {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        Node* temp = topNode;
        T value = topNode->data;
        topNode = topNode->next;
        delete temp;
        size--;
        return value;
    }

    // Peek at top element
    T peek() const {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        return topNode->data;
    }

    // Check if stack is empty
    bool isEmpty() const {
        return topNode == nullptr;
    }

    // Get size of stack
    int getSize() const {
        return size;
    }

    // Clear the stack
    void clear() {
        while (!isEmpty()) {
            pop();
        }
    }
};

#endif // STACK_H
