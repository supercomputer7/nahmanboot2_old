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
    Node<T>* get_last_node()
    {
        return this->tail;
    }
    uint32_t get_count()
    {
        return this->count;
    }
    void initialize(Node<T>* list,uint32_t count)
    {
        this->count = count;
        this->list = list;
        this->tail = list;
    }
    void insert_node(T* object)
    {
        if(this->list == nullptr)
        {
            this->list = new Node<T>();
            this->tail = list;
        }
        else
        {
            tail->set_next(new Node<T>());
            this->tail = tail->get_next();
        }
        tail->initialize(object,nullptr);
        this->count++;
    }
    void adpot_node(Node<T>* node)
    {
        if(this->list == nullptr)
        {
            this->list = node;
            this->tail = list;
        }
        else
        {
            tail->set_next(node);
        }
        this->count++;
    }
    void adpot_list(List<T>* list)
    {
        for(uint32_t i=0; i < list->get_count(); i++)
        {
            this->insert_node(list->get_node(i)->get_object());
        }
    }
private:
    uint32_t count;
    Node<T>* list;
    Node<T>* tail;
};