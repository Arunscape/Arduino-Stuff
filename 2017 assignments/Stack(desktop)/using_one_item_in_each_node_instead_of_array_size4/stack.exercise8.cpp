#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "stack.exercise8.h"

Stack::Stack() {
  //start with empty linked list
  head=NULL;
}

Stack::~Stack() {
  //while list isn't empty , keep popping
  while (head != NULL){
    pop();
  }
}

bool Stack::isEmpty() {
  //if head == NULL; then list is empty
  return head == NULL;
}

int Stack::top() {
  assert(!isEmpty());
  //head points to the first node, access that node's data
  return head->data;
}

void Stack::pop() {
  assert(!isEmpty());
  //make variable temp1 whicn is the node that
  //head currently points to
  //then set head = the node that temp1.next
  //points to, which is the next node after
  Node* temp1 = head;
  head= temp1->next;
  //then free up memory by deleting unlinked node, which used to be
  //what head pointed to before the pop
  delete temp1;


}

void Stack::push(int item) {
  // Node* tmp = (Node*)malloc(sizeof(Node));
  // (*tmp).data=item;
  // (*tmp).next =NULL;
  //^^ that's the C way of doing it

  //below is C++ way of creating new node
  Node* tmp = new Node();
  //temp.data = item to add
  //temp.next = current head
  tmp -> data=item;
  tmp -> next=head;
  //now make head point to the new node
  head=tmp;
}
