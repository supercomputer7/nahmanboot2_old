#pragma once
#include <stdint.h>
#include <LibC/stdbool.h>
#include <Memory/Node.h>
#include <Memory/malloc.h>

class Stack {

public:
    ~Stack();
    void initialize();
    void* pop();
    void push(void* object);
    Node* top();
    bool isEmpty();
private:
    Node* head;
};
