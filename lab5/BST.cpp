//
//  BST.cpp
//  Lab 5 Search Through A Database
//
//  Created by Harry Zhang, 2025-11-11
#include "BST.h"

#include "Employee.h"

BST::BST(string order_) {
  root = NULL;
  order = order_;
}

BST::~BST() { delete root; }

void insertByIDHelper(Employee* newEmployee, Employee* node) {
  if (newEmployee->getID() < node->getID()) {
    if (node->getLeft() == nullptr) {
      node->setLeft(newEmployee);
    } else {
      insertByIDHelper(newEmployee, node->getLeft());
    }
  } else {
    if (node->getRight() == nullptr) {
      node->setRight(newEmployee);
    } else {
      insertByIDHelper(newEmployee, node->getRight());
    }
  }
}

void insertByAgeHelper(Employee* newEmployee, Employee* node) {
  if (newEmployee->getAge() <= node->getAge()) {
    if (node->getLeft() == nullptr) {
      node->setLeft(newEmployee);
    } else {
      insertByAgeHelper(newEmployee, node->getLeft());
    }
  } else {
    if (node->getRight() == nullptr) {
      node->setRight(newEmployee);
    } else {
      insertByAgeHelper(newEmployee, node->getRight());
    }
  }
}

void insertByNameHelper(Employee* newEmployee, Employee* node) {
  if (newEmployee->getName() < node->getName()) {
    if (node->getLeft() == nullptr) {
      node->setLeft(newEmployee);
    } else {
      insertByNameHelper(newEmployee, node->getLeft());
    }
  } else {
    if (node->getRight() == nullptr) {
      node->setRight(newEmployee);
    } else {
      insertByNameHelper(newEmployee, node->getRight());
    }
  }
}

// newEmployee is a pointer to a dynamically allocated Employee. Insert it
// according to the value of "order" of the binary search tree.
void BST::insert(Employee* newEmployee) {
  // insert by ID
  if (order == "ID") {
    if (root == nullptr) {  // insert at root
      root = newEmployee;
    } else {
      insertByIDHelper(newEmployee, root);
    }
  }
  if (order == "name") {
    if (root == nullptr) {  // insert at root
      root = newEmployee;
    } else {
      insertByNameHelper(newEmployee, root);
    }
  }
  if (order == "age") {
    if (root == nullptr) {  // insert at root
      root = newEmployee;
    } else {
      insertByAgeHelper(newEmployee, root);
    }
  }
}

// print the Employees in the tree according its order
// Doesn't print anything if nothing is found
void printInOrderHelper(Employee* node) {
  if (node != nullptr) {
    printInOrderHelper(node->getLeft());
    node->print();
    printInOrderHelper(node->getRight());
  }
}

void BST::printInOrder() { printInOrderHelper(root); }

// search for an employee with a particular ID
// if ID doesn't exist, it returns NULL
Employee* searchIdHelper(int ID, Employee* node) {
  if (node == nullptr)
    return nullptr;
  else if (node->getID() == ID) {
    return node;
  } else if (node->getID() > ID)  // search left subtree
    return searchIdHelper(ID, node->getLeft());
  else
    return searchIdHelper(ID, node->getRight());  // search right subtree
}

Employee* BST::searchID(int ID) { return searchIdHelper(ID, root); }

void searchAgeRangeHelper(double lowAge, double highAge, Employee* node) {
  if (node != nullptr) {
    if (lowAge <= node->getAge()) 
      searchAgeRangeHelper(
          lowAge, highAge,
          node->getLeft());  // get the left most node thats within the range
      if (lowAge <= node->getAge() && node->getAge() <= highAge) node->print();
      if (node->getAge() < highAge)
        searchAgeRangeHelper(lowAge, highAge, node->getRight());
    
  }
}

// search for employees within the age range
// Doesn't print anything if nothing is found
void BST::searchAgeRange(double lowAge, double highAge) {
  searchAgeRangeHelper(lowAge, highAge, root);
}

// Search for employees with names having the same prefix in the firstName +
// secondName If available, it prints them in order of their names in
// alphabetical order
// Doesn't print anything if nothing is found

bool findprefix(string& prefix, Employee* node) {
  string name = node->getName();
  return name.find(prefix) == 0;
}

void autocompleteHelper(string& prefix, Employee* node) {
  if (node != nullptr) {
    autocompleteHelper(prefix, node->getLeft());
    if (findprefix(prefix, node)) node->print();
    autocompleteHelper(prefix, node->getRight());
  }
}

void BST::autocomplete(string prefix) { autocompleteHelper(prefix, root); }
