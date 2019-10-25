#pragma once
#include <stdint.h>
#include <Memory/malloc.h>
#include <Memory/Node.h>

template<typename T> 
class List {

public:
    ~List();  
    List(Node<T>* list,uint32_t count)
    {
        this->count = count;
        this->list = list;
    }
    Node<T>* get_node(uint32_t node)
    {
        Node<T>* tmp = this->list;
        uint32_t i=0;
        while(i<node)
        {
            if(tmp->get_next() == nullptr)
                return nullptr;
            tmp = tmp->get_next();
            i++;
        }
        return tmp;
    }
    uint32_t get_count()
    {
        return this->count;
    }
    void initialize(Node<T>* list,uint32_t count)
    {
        this->count = count;
        this->list = list;
    }
private:
    uint32_t count;
    Node<T>* list;
};