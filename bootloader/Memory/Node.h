#pragma once
#include <stdint.h>
#include <Memory/malloc.h>

class Node {

public:
    ~Node();
    void initialize(void* object, Node* next);
    Node* get_next();
    void* get_object();
    void set_next(Node* next);
private:
    Node* next;
    void* object;
};
