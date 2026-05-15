#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>

template<typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;
        
        Node(T val) : data(val), next(nullptr) {}
    };
    
    Node* head;
    int size;

public:
    LinkedList() : head(nullptr), size(0) {}

    ~LinkedList() {
        clear();
    }

    // Insert at the beginning
    void insert(T value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;
        size++;
    }

    // Insert at the end
    void insertAtEnd(T value) {
        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = newNode;
        } else {
            Node* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
        size++;
    }

    // Remove a specific value
    bool remove(T value) {
        if (head == nullptr) return false;
        
        if (head->data == value) {
            Node* temp = head;
            head = head->next;
            delete temp;
            size--;
            return true;
        }
        
        Node* current = head;
        while (current->next != nullptr) {
            if (current->next->data == value) {
                Node* temp = current->next;
                current->next = current->next->next;
                delete temp;
                size--;
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // Search for a value
    bool search(T value) const {
        Node* current = head;
        while (current != nullptr) {
            if (current->data == value) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // Get size
    int getSize() const {
        return size;
    }

    // Check if empty
    bool isEmpty() const {
        return head == nullptr;
    }

    // Clear the list
    void clear() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        size = 0;
    }

    // Iterator support for range-based loops
    class Iterator {
    private:
        Node* current;
    public:
        Iterator(Node* node) : current(node) {}
        
        T& operator*() { return current->data; }
        Iterator& operator++() { 
            current = current->next; 
            return *this; 
        }
        bool operator!=(const Iterator& other) const { 
            return current != other.current; 
        }
    };

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }
};

#endif // LINKEDLIST_H
