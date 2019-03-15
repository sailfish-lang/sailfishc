/**
 * Do not alter! This code is generated by the sailfishc
 * compiler and might/will break if anything changed.
 */
#include "stdlib/stdlib.h"

typedef struct _Node_ {
    struct _Node_* next;
    int data;
} Node;

Node*
construct_Node(Node* next,int data)
{
    Node* a____struct___generated = (Node*)malloc(sizeof(Node));
    a____struct___generated->next = next;    a____struct___generated->data = data;
    return a____struct___generated;
}

int
has_next(Node* own)
{
    int ret = (own->next!=NULL);
    return ret;
}
void
set_next(Node* own, Node* node)
{
    own->next = node;
}
Node*
next(Node* own)
{
    return own->next;
}
int
data(Node* own)
{
    return own->data;
}
typedef struct _Stack_ {
    struct _Node_* head;
    int size;
} Stack;

Stack*
construct_Stack(Node* head,int size)
{
    Stack* a____struct___generated = (Stack*)malloc(sizeof(Stack));
    a____struct___generated->head = head;    a____struct___generated->size = size;
    return a____struct___generated;
}

int
is_empty(Stack* own)
{
    return (own->size==0);
}
int
size(Stack* own)
{
    return own->size;
}
int
peek(Stack* own)
{
    Node*temp = construct_Node(NULL,0);
    temp = own->head;
    return temp->data;
}
void
push(Stack* own, Node* node)
{
    if(is_empty==0)
{
    own->head = node;
}
else 
{
    set_next(node, own->head);
    own->head = node;
}

    own->size = own->size+1;
}
void
pop(Stack* own)
{
    if(is_empty==1)
{

}
else 
{
    Node*temp = construct_Node(NULL,0);
    temp = own->head;
    own->head = next(temp);
    own->size = own->size-1;
}

}
void
print_(Stack* own, Node* node)
{
    if(node!=NULL)
{
    print_int(data(node));
    print_str(" ");
    print_(own, next(node));
}
else 
{

}

}
void
print(Stack* own)
{
    print_str("Stack contents: ");
    print_(own, own->head);
    print_str("\n");
}
int
main()
{
    Node*a = construct_Node(NULL,1);
    Node*b = construct_Node(NULL,2);
    Node*c = construct_Node(NULL,3);
    Stack*s = construct_Stack(NULL,0);
    push(s, a);
    push(s, b);
    push(s, c);
    print(s);
    print_str("Size: ");
    print_int(size(s));
    print_str("\tTop:");
    print_int(peek(s));
    print_str("\n");
    pop(s);
    pop(s);
    print_str("Size: ");
    print_int(size(s));
    print_str("\tTop:");
    print_int(peek(s));
    print_str("\n");
}
