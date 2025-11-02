#include "QueueList.h"

#include "Customer.h"

QueueList::QueueList() { head = nullptr; }

QueueList::QueueList(Customer* customer) { head = customer; }

QueueList::~QueueList() { }

Customer* QueueList::get_head() { return head; }

void QueueList::enqueue(Customer* customer) {
  if (head == nullptr) {
    head = customer; // queue is empty
  } else {
    Customer* current = head;
    while (current->get_next() != nullptr) {
      current = current->get_next();
    }
    current->set_next(customer);
  }

  // a customer is placed at the end of the queue
  // if the queue is empty, the customer becomes the head
}

Customer* QueueList::dequeue() {
  if (head == nullptr){
     return nullptr;
  }
  Customer* current = head;
  head = current->get_next();
  current->set_next(nullptr);
  return current;
  // remove a customer from the head of the queue
  // and return a pointer to it
}

int QueueList::get_items() {
  int counter = 0;
  Customer* current = head;
  while (current != nullptr) {
    counter += current->get_numOfItems();
    current = current->get_next();
  }
  return counter;
  // count total number of items each customer in the queue has
}

void QueueList::print() {
  // print customers in a queue
  Customer* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}
