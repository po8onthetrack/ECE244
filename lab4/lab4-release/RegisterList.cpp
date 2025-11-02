#include "RegisterList.h"

#include <iostream>

#include "Register.h"
using namespace std;

RegisterList::RegisterList() {
  head = nullptr;
  size = 0;
}

RegisterList::~RegisterList() {
  delete head;
  head = nullptr;
  // Delete all registers in the list
}

Register* RegisterList::get_head() {return head;}

int RegisterList::get_size() { 
  return size;
  // return number of registers 
}


Register* RegisterList::get_min_items_register() {
  Register* current = head;
  if(current -> get_next() == nullptr)
  return current; // only one register
  else{
  int min= current -> get_queue_list() -> get_items();
  Register* min_reg = nullptr;
  while(current != nullptr){
    int current_itme = current -> get_queue_list() -> get_items();
    if(current_itme < min){
      min = current_itme; 
      min_reg = current;
    }
    current = current -> get_next();
  }
  return min_reg;
}
  // loop all registers to find the register with least number of items
  
}

Register* RegisterList::get_free_register() {
  Register* current = head;
  if(head ==  nullptr)
  return nullptr;
  while(current != nullptr){
    if(current -> get_queue_list() -> get_head() == nullptr)
    return current;
  }
  return nullptr;
  // return the register with no customers
  // if all registers are occupied, return nullptr
}

void RegisterList::enqueue(Register* newRegister) {
  if(head == nullptr){
    head = newRegister;
    size ++;
  }
  else{
    Register* current = head;
    while(current -> get_next() != nullptr){
      current = current -> get_next();
    }
    current -> set_next(newRegister);
    size ++;
  }
  // a register is placed at the end of the queue
  // if the register's list is empty, the register becomes the head
  // Assume the next of the newRegister is set to null
  // You will have to increment size 
  
}

bool RegisterList::foundRegister(int ID) {
  if(head == nullptr)
  return false; // no register
  else{
    Register* current = head;
    while(current != nullptr){
    if(current -> get_ID() == ID)
    return true; 
  }
  current = current -> get_next();
  }
  return false;
  // look for a register with the given ID
  // return true if found, false otherwise
}

Register* RegisterList::dequeue(int ID) {
  Register* prev = head;
  Register* current = head -> get_next();
  if(foundRegister(ID)){
    if(head -> get_ID() == ID){
      head = head -> get_next();
      prev -> set_next(nullptr);
      return prev;
    }// dequeue first register
    else{
      while(current != nullptr){
        if(current -> get_ID() == ID){
          prev -> set_next(current -> get_next());
          current -> set_next(nullptr);
          return current;
        }
      }
      prev = current;
      current = current -> get_next();
    }
    size --;
  }
  return nullptr;
  // dequeue the register with given ID
  // return the dequeued register
  // return nullptr if register was not found
}

Register* RegisterList::calculateMinDepartTimeRegister(double expTimeElapsed) {
  if(head == nullptr) return nullptr;
  //no register
  Register* min_reg = nullptr; 
  Register* current = head;
  bool allRegisterEmpty = true;
  double mindeparttime = head -> calculateDepartTime(); //set mindepart time equal to first register
  while(current != nullptr){
    double departTime = current -> calculateDepartTime();
    if(departTime != -1)
    allRegisterEmpty = false;
    if(departTime != -1 && (mindeparttime == -1 || departTime < mindeparttime)){
      mindeparttime = departTime;
      min_reg = current;
    }
    current = current -> get_next();
  }
  if(allRegisterEmpty)
      return nullptr; // all register empty
  else
    return min_reg;
  // return the register with minimum time of departure of its customer
  // if all registers are free, return nullptr
}

void RegisterList::print() {
  Register* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}
