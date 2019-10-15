#include <Memory/Node.h>

void Node::initialize(void* object, Node* next)
{
    this->object = object;
    this->next = next;
}
Node* Node::get_next()
{
    return this->next;
}
void* Node::get_object()
{
    return this->object;
}
void Node::set_next(Node* next)
{
    this->next = next;
}