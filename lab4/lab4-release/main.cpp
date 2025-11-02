#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "Customer.h"
#include "QueueList.h"
#include "Register.h"
#include "RegisterList.h"

using namespace std;

// Function Declarations:

// Set mode of the simulation
string getMode();

// Register
void parseRegisterAction(stringstream &lineStream, string mode);
void openRegister(
    stringstream &lineStream,
    string mode);  // register opens (it is upto customers to join)
void closeRegister(stringstream &lineStream,
                   string mode);  // register closes

// Customer
void addCustomer(stringstream &lineStream,
                 string mode);  // customer wants to join

void departCustomers(string mode);

// Helper functions
bool getInt(stringstream &lineStream, int &iValue);
bool getDouble(stringstream &lineStream, double &dValue);
bool foundMoreArgs(stringstream &lineStream);

// Global variables
RegisterList *registerList;  // holding the list of registers
QueueList *doneList;         // holding the list of customers served
QueueList *singleQueue;      // holding customers in a single virtual queue
double expTimeElapsed;  // time elapsed since the beginning of the simulation

// List of commands:
// To open a register
// register open <ID> <secPerItem> <setupTime> <timeElapsed>
// To close register
// register close <ID> <timeElapsed>
// To add a customer
// customer <items> <timeElapsed>

int main() {
  registerList = new RegisterList();
  doneList = new QueueList();
  singleQueue = new QueueList();
  expTimeElapsed = 0;

  // Set mode by the user
  string mode = getMode();

  string line;
  string command;
  cout << "> ";  // Prompt for input
  getline(cin, line);
  while (!cin.eof()) {
    stringstream lineStream(line);
    lineStream >> command;
    if (command == "register") {
      parseRegisterAction(lineStream, mode);
    } else if (command == "customer") {
      addCustomer(lineStream, mode);
    } else {
      cout << "Invalid operation" << endl;
    }
    cout << "> ";  // Prompt for input
    getline(cin, line);
  }
  cout<<endl<<"Finished at time "<< expTimeElapsed <<endl;

  
  
  if(doneList -> get_head()!= nullptr){
  double maxWaitT = 0;
  double aveWaitT = 0;
  double stdDev = 0;
  int numCustomer = 0;
  double totalWaitT = 0;
  Customer* temp1 = doneList -> get_head();
  while(temp1 != nullptr){
    double waitT = temp1 -> get_departureTime() - temp1 -> get_arrivalTime();
    if(maxWaitT < waitT){
      maxWaitT = waitT;
    }
    totalWaitT += waitT;
    numCustomer ++;
    temp1 = temp1 -> get_next();
  }
  aveWaitT = totalWaitT/numCustomer;

  Customer* temp2 = doneList -> get_head();
  double sumsquareDiff = 0;
  while(temp2 != nullptr){
    double waitT = temp2 -> get_departureTime() - temp2 -> get_arrivalTime();
    sumsquareDiff += pow((waitT - aveWaitT) , 2);
    temp2 = temp2 -> get_next();
  }
  stdDev = sqrt(sumsquareDiff/numCustomer);
    cout<<"Statistics: "<<endl
    <<"Maximum wait time: "<< maxWaitT <<endl
    <<" Average wait time: "<< aveWaitT <<endl
    <<" Standard Deviation of wait time: "<< stdDev <<endl;
  }

   while (doneList->get_head() != nullptr) {
    Customer* customer = doneList->dequeue();
    delete customer;
  }
  delete doneList;// delete donelist
  while(singleQueue -> get_head() != nullptr){
    Customer* customer = singleQueue -> dequeue();
    delete customer;
  }
  delete singleQueue;// delete singleQueue
  delete registerList;
  // You have to make sure all dynamically allocated memory is freed
  // before return 0
  return 0;
}

string getMode() {
  string mode;
  cout << "Welcome to ECE 244 Grocery Store Queue Simulation!" << endl;
  cout << "Enter \"single\" if you want to simulate a single queue or "
          "\"multiple\" to "
          "simulate multiple queues: \n> ";
  getline(cin, mode);

  if (mode == "single") {
    cout << "Simulating a single queue ..." << endl;
  } else if (mode == "multiple") {
    cout << "Simulating multiple queues ..." << endl;
  }

  return mode;
}

void addCustomer(stringstream &lineStream, string mode) {
  int items;
  double timeElapsed;

  if (!getInt(lineStream, items) || !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments." << endl;
    return;
  }
  expTimeElapsed += timeElapsed;
  departCustomers(mode);
  Customer *customer = new Customer(expTimeElapsed, items);
  cout << "A customer entered" << endl;

  if (mode == "single") {
    Register* freeRegister = registerList -> get_free_register();
    if(freeRegister != nullptr){
      freeRegister -> get_queue_list() -> enqueue(customer); 
      cout << "Queued a customer with free register " << freeRegister->get_ID()
           << endl;// add customer to a free register
    }else{
      singleQueue->enqueue(customer);
      cout << "No free registers"
           << endl; // add customers to the single queue

    } 
  } else if (mode == "multiple") {
    Register* minItemReg = registerList -> get_min_items_register();
    if (minItemReg != nullptr) {
    minItemReg -> get_queue_list() -> enqueue(customer);
    cout<<"Queued a customer with quickest register "<< 
    minItemReg -> get_ID()<<endl;
  } // add the customer to the registre with fewest items
  }
}

void departCustomers(string mode) {
  // depart customers at each register whose process time has passed
  while (true) {
    Register *min_reg =
        registerList->calculateMinDepartTimeRegister(expTimeElapsed);
    if (min_reg ==
        nullptr)  // all register is empty, no customer can be departed
      return;

    Customer *customer = min_reg->get_queue_list()->get_head();
    if(customer == nullptr) return;

    double departTime = min_reg->calculateDepartTime();

    if (departTime > expTimeElapsed)
      return;  // no customer can be departed at this moment

    min_reg->departCustomer(doneList);  // this will update the available time for the register
    cout << "Departed a customer at register ID " << min_reg->get_ID() << " at "
         << departTime << endl;

    if (mode == "single" && singleQueue->get_head() != nullptr) {
      Customer *nextCustomer = singleQueue->dequeue();
      min_reg->get_queue_list()->enqueue(nextCustomer);
      cout << "Queued a customer with free register " 
         << min_reg->get_ID() << endl;
    }  // if simulation is single, join next customer to the register
  }
}

void parseRegisterAction(stringstream &lineStream, string mode) {
  string operation;
  lineStream >> operation;
  if (operation == "open") {
    openRegister(lineStream, mode);
  } else if (operation == "close") {
    closeRegister(lineStream, mode);
  } else {
    cout << "Invalid operation" << endl;
  }
}

void openRegister(stringstream &lineStream, string mode) {
  int ID;
  double secPerItem, setupTime, timeElapsed;
  // convert strings to int and double
  if (!getInt(lineStream, ID) || !getDouble(lineStream, secPerItem) ||
      !getDouble(lineStream, setupTime) ||
      !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }
  if (registerList->foundRegister(ID)) {
    cout << "Error: register " << ID << " is already open" << endl;
    return;
  }
  
  expTimeElapsed += timeElapsed;
  departCustomers(mode);

  // Check if the register is already open If it's open, print an error message
  Register *newRegister = new Register(ID, secPerItem, setupTime, expTimeElapsed);
  registerList->enqueue(newRegister);
  cout << "Opened register " << ID << endl;

  

  // Otherwise, open the register
  if (mode == "single" && singleQueue->get_head() != nullptr) {
    Customer *customer = singleQueue->dequeue();
    newRegister->get_queue_list()->enqueue(customer);
    cout << "Queued a customer with free register " << ID << endl;
  }
  // If we were simulating a single queue,
  // and there were customers in line, then
  // assign a customer to the new register
}

void closeRegister(stringstream &lineStream, string mode) {
  int ID;
  double timeElapsed;
  // convert string to int
  if (!getInt(lineStream, ID) || !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }

 
  if(! registerList -> foundRegister(ID)){ // Check if the register is open
    cout<<" Error: register "<< ID <<" is not open"<<endl;
    return; // not open
  }
  expTimeElapsed += timeElapsed;
  departCustomers(mode);

  Register* regClose = registerList -> dequeue(ID);
  delete regClose;
  // If it is open dequeue it and free it's memory
   cout << "Closed register " << ID << endl;
}

bool getInt(stringstream &lineStream, int &iValue) {
  // Reads an int from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  iValue = stoi(command);
  return true;
}

bool getDouble(stringstream &lineStream, double &dvalue) {
  // Reads a double from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  dvalue = stod(command);
  return true;
}

bool foundMoreArgs(stringstream &lineStream) {
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  } else {
    return true;
  }
}
