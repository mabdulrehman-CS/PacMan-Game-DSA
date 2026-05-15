#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>

template<typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;
        
        Node(T val) : data(val), next(nullptr) {}
    };
    
    Node* frontNode;
    Node* rearNode;
    int size;

public:
    Queue() : frontNode(nullptr), rearNode(nullptr), size(0) {}

    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    // Enqueue element
    void enqueue(T value) {
        Node* newNode = new Node(value);
        if (isEmpty()) {
            frontNode = rearNode = newNode;
        } else {
            rearNode->next = newNode;
            rearNode = newNode;
        }
        size++;
    }

    // Dequeue element
    T dequeue() {
        if (isEmpty()) {
            throw std::runtime_error("Queue is empty");
        }
        Node* temp = frontNode;
        T value = frontNode->data;
        frontNode = frontNode->next;
        
        if (frontNode == nullptr) {
            rearNode = nullptr;
        }
        
        delete temp;
        size--;
        return value;
    }

    // Get front element
    T front() const {
        if (isEmpty()) {
            throw std::runtime_error("Queue is empty");
        }
        return frontNode->data;
    }

    // Check if queue is empty
    bool isEmpty() const {
        return frontNode == nullptr;
    }

    // Get size of queue
    int getSize() const {
        return size;
    }

    // Clear the queue
    void clear() {
        while (!isEmpty()) {
            dequeue();
        }
    }
};

#endif // QUEUE_H
