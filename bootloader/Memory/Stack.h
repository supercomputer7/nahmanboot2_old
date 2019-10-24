#pragma once
#include <stdint.h>
#include <LibC/stdbool.h>
#include <Memory/Node.h>
#include <Memory/malloc.h>

template <typename T> 
class Stack {

public:
    ~Stack();
    void initialize()
    {
        head = nullptr;
    }
    T* pop()
    {
        T* tmp = head->get_object();
        head = head->get_next();
        return tmp;
    }
    void push(T* object)
    {
        Node<T> node = new Node<T>();
        node->initialize(object,nullptr);
        node->set_next(head);
        head = node;
    }
    Node<T>* top()
    {
        return this->head;
    }
    bool isEmpty()
    {
        return head == nullptr;
    }
private:
    Node<T>* head;
};