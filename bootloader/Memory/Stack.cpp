#include <Memory/Stack.h>

void Stack::initialize()
{
    head = nullptr;
}
void* Stack::pop()
{
    void* tmp = head->get_object();
    head = head->get_next();
    return tmp;
}
void Stack::push(void* object)
{
    Node* node = new Node();
    node->initialize(object,nullptr);
    node->set_next(head);
    head = node;
}
Node* Stack::top()
{
    return this->head;
}
bool Stack::isEmpty()
{
    return head == nullptr;
}
