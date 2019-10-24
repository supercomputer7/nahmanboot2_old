#pragma once
#include <stdint.h>
#include <Memory/malloc.h>

template<typename T> 
class Node {

public:
    ~Node();    
    Node<T>* get_next()
    {
        return this->next;
    }
    T* get_object()
    {
        return this->object;
    }
    void set_next(Node<T>* next)
    {
        this->next = next;
    }
    void initialize(T* object, Node<T>* next)
    {
        this->object = object;
        this->next = next;
    }
private:
    
    Node* next;
    T* object;
};