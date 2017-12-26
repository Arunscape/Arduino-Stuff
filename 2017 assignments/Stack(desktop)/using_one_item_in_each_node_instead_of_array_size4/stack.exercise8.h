#ifndef STACK_H
#define STACK_H

//struct for linked list
struct Node{
  int data;
  Node* next;
};


class Stack {
public:
  //declare head pointer of linked list
  Node* head;

  Stack();
  ~Stack();
  bool isEmpty();
  int top();
  void pop();
  void push(int item);
};

#endif
