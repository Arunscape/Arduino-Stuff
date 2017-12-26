#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "stack.exercise8.h"

Stack::Stack() {
  //start with empty linked list
  head=NULL;

  //array of size 4 for each block
  topIdx=-1;
  arrSize=3;
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
  return head->data[topIdx];
}

void Stack::pop() {
  assert(!isEmpty());

  topIdx--;
  if(topIdx<0){
    //make variable temp1 whicn is the node that
    //head currently points to
    //then set head = the node that temp1.next
    //points to, which is the next node after
    Node* temp1 = head;
    head= temp1->next;
    //then free up memory by deleting unlinked node, which used to be
    //what head pointed to before the pop
    delete temp1;


    //different index positions depending on whether the last node was popped
    //or just one node that wasn't the last one
    if(head==NULL){
      topIdx=-1;
    }
    else{
      topIdx=3;
    }
  }

}

void Stack::push(int item) {
  topIdx++;

  if(head==NULL){//new node if list is empty
    Node* tmp = new Node();
    tmp -> data[topIdx]=item;
    tmp -> next = head;
    head=tmp;
    head->data[topIdx]=item;
    return;
  }
  if(topIdx > arrSize){//make new storage block

    // Node* tmp = (Node*)malloc(sizeof(Node));
    // (*tmp).data=item;
    // (*tmp).next =NULL;
    //^^ that's the C way of doing it

    //below is C++ way of creating new node
    Node* tmp = new Node();
    //temp.data = item to add
    //temp.next = current head
    tmp -> data[0]=item;
    tmp -> next=head;
    //now make head point to the new node
    head=tmp;
    topIdx=0;
    head->data[0]=item;
    return;
  }
//happens when there's spece in the array
head->data[topIdx]=item;
return;

}
