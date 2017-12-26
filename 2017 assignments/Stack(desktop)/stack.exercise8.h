#ifndef STACK_H
#define STACK_H

//struct for linked list
struct Node{
  int data[4];
  Node* next;
};


class Stack {
public:
  //declare head pointer of linked list
  Node* head;

  //variables that keep track of array index and size
  int topIdx;
  int arrSize;


  //stack methods
  Stack();
  ~Stack();
  bool isEmpty();
  int top();
  void pop();
  void push(int item);
};

#endif
