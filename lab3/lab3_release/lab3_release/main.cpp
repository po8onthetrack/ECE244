//
//  main.cpp
//  Lab 3 The Resistor Network Program
//
//  Created by Nathan Hung on 2024-09-11.
//  Modified by Salma Emara on 2024-09-30
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.
//
//  ECE244 Student: Complete the skeletal code of the main function in this file

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "Node.h"
#include "Resistor.h"

using namespace std;
#define MIN_ITERATION_CHANGE 0.0001

Resistor** resistors =
    nullptr;  // Pointer that should point to an array of Resistor pointers
Node* nodes = nullptr;  // pointer that should hold address to an array of Nodes
int maxNodeNumber = 0;  // maximum number of nodes as set in the command line
int maxResistors = 0;  // maximum number of resistors as set in the command line
int resistorsCount = 0;  // count the number of resistors

string errorArray[10] = {
    "invalid command",                                  // 0
    "invalid argument",                                 // 1
    "negative resistance",                              // 2
    "node value is out of permitted range",             // 3
    "resistor name cannot be keyword \"all\"",          // 4
    "both terminals of resistor connect to same node",  // 5
    "too few arguments",                                // 6
};

// Function Prototypes
bool getInteger(stringstream& ss, int& x);
bool getString(stringstream& ss, string& s);
bool getDouble(stringstream& ss, double& s);
bool resistancePos(const double& x);
bool resistanceNameAll(const string& s);
bool nameExsists(const string& s);
int findIndexByName(const string& s);
bool notSameNode(const int& x, const int& y);
bool nodeInRange(const int& nodeid);

void handleMaxVal(stringstream& ss);
void handleInsertR(stringstream& ss);
void handleModifyR(stringstream& ss);
void handlePrintR(stringstream& ss);
void handleDeleteR(stringstream& ss);
void handleSetV(stringstream& ss);
void solve();

int main() {
  string line;
  while (!cin.eof()) {
    cout << ">>> ";
    cout.flush();
    // TODO: Implement the main function here
    getline(cin, line);
    if (cin.eof()) break;
    stringstream myss(line);
    string command;
    myss >> command;
    if (command == "maxVal")
      handleMaxVal(myss);
    else if (command == "insertR")
      handleInsertR(myss);
    else if (command == "modifyR")
      handleModifyR(myss);
    else if (command == "printR")
      handlePrintR(myss);
    else if (command == "deleteR")
      handleDeleteR(myss);
    else if (command == "setV")
      handleSetV(myss);
    else if (command == "solve")
      solve();
    else
      cout << "Error: invalid command" << endl;
  }
  if (resistors != nullptr) {
    for (int i = 0; i < resistorsCount; ++i) delete resistors[i];
    delete[] resistors;
  }
  if (nodes != nullptr) delete[] nodes;
  return 0;
}

// TODO: Implement functions here
bool getInteger(stringstream& ss, int& x) {
  ss >> x;
  if (ss.fail()) {
    if (ss.eof()) {
      cout << "Error: too few arguments" << endl;
      return false;
    } else {
      cout << "Error: invalid argument" << endl;
      ss.clear();
      return false;
    }
  }
  return true;
}

bool getString(stringstream& ss, string& s) {
  ss >> s;
  if (ss.fail()) {
    if (ss.eof()) {
      cout << "Error: too few arguments" << endl;
      return false;
    } else {
      cout << "Error: invalid argument" << endl;
      ss.clear();
      return false;
    }
  }
  return true;
}

bool getDouble(stringstream& ss, double& s) {
  ss >> s;
  if (ss.fail()) {
    if (ss.eof()) {
      cout << "Error: too few arguments" << endl;
      return false;
    } else {
      cout << "Error: invalid argument" << endl;
      ss.clear();
      return false;
    }
  }
  return true;
}

bool resistancePos(const double& x) {
  if (x < 0) {
    cout << "Error: negative resistance" << endl;
    return false;
  }
  return true;
}

bool resistanceNameAll(const string& s) {
  if (s == "all") {
    cout << "Error: resistor name cannot be the keyword \"all\"" << endl;
    return true;
  }
  return false;
}

bool nameExsists(const string& s) {
  for (int i = 0; i < resistorsCount; ++i) {
    if (resistors[i]->getName() == s) {
      cout << "Error: resistor " << s << " already exists" << endl;
      return true;
    }
  }
  return false;
}

bool notSameNode(const int& x, const int& y) {
  if (x == y) {
    cout << "Error: both terminals of resistor connect to same node" << endl;
    return false;
  }
  return true;
}

bool nodeInRange(const int& nodeid) {
  if (nodeid < 0 || nodeid > maxNodeNumber) {
    cout << "Error: node value is out of permitted range 1 - " << maxNodeNumber
         << endl;
    return false;
  }
  return true;
}

int findIndexByName(const string& s) {  // returns the index of the resistor if
                                        // found(for modifyR and printR)
  for (int i = 0; i < resistorsCount; ++i) {
    if (resistors[i]->getName() == s) return i;
  }
  cout << "Error: resistor " << s << " not found" << endl;
  return -1;
}

void handleMaxVal(stringstream& ss) {
  // check error
  if (getInteger(ss, maxNodeNumber) && getInteger(ss, maxResistors)) {
    // check if the node and resistor arrays are empty
    if (nodes != nullptr) {
      delete[] nodes;
      nodes = nullptr;
    }
    if (resistors != nullptr) {
      for (int i = 0; i < resistorsCount; ++i) {
        delete resistors[i];
        resistors[i] = nullptr;
      }
      delete[] resistors;
      resistors = nullptr;
    }
    // set the max value
    nodes = new Node[maxNodeNumber];
    resistors = new Resistor*[maxResistors];
    resistorsCount = 0;
    cout << "New network: max node number is " << maxNodeNumber
         << "; max resistors is " << maxResistors << endl;
  }
}

void handleInsertR(stringstream& ss) {
  string nameResis;
  double resistance_;
  int nodeid1, nodeid2;
  if (getString(ss, nameResis) && getDouble(ss, resistance_) &&
      getInteger(ss, nodeid1) && getInteger(ss, nodeid2)) {
    // check error
    if (!resistancePos(resistance_)) return;
    if (resistanceNameAll(nameResis)) return;
    if (nameExsists(nameResis)) return;
    if (notSameNode(nodeid1, nodeid2) && nodeInRange(nodeid1) &&
        nodeInRange(nodeid2)) {
      nodeid1--;
      nodeid2--;  // nodeid is 1 based on the user input
      int twoNode[2] = {nodeid1, nodeid2};
      resistors[resistorsCount] = new Resistor(nameResis, resistance_, twoNode);
      nodes[nodeid1].addResistor(resistorsCount);
      nodes[nodeid2].addResistor(resistorsCount);
      resistorsCount++;
      cout << "Inserted: resistor " << nameResis << " " << fixed
           << setprecision(2) << resistance_ << " Ohms " << nodeid1 + 1
           << " -> " << nodeid2 + 1 << endl;
    }
  }
}

void handleModifyR(stringstream& ss) {
  string nameResis;
  double newResistance_;
  if (getString(ss, nameResis) && getDouble(ss, newResistance_)) {
    if (!resistancePos(newResistance_)) return;
    if (resistanceNameAll(nameResis)) return;
    int index = findIndexByName(nameResis);
    if (index == -1) return;
    double oldResis = resistors[index]->getResistance();
    resistors[index]->setResistance(newResistance_);
    cout << fixed << setprecision(2) << "Modified: resistor " << nameResis
         << " from " << oldResis << " Ohms to " << newResistance_ << " Ohms"
         << endl;
  }
}

void handlePrintR(stringstream& ss) {
  string nameResis;
  if (getString(ss, nameResis)) {
    int index = findIndexByName(nameResis);
    if (index == -1) return;
    cout << "Print: " << endl;
    resistors[index]->print();
  }
}

void handleDeleteR(stringstream& ss) {
  string del;
  if (getString(ss, del)) {
    for (int i = 0; i < resistorsCount; ++i) {
      delete resistors[i];
      resistors[i] = nullptr;
    }
    resistorsCount = 0;
    for (int j = 0; j < maxNodeNumber; ++j) {
      nodes[j] = Node();
    }
    cout << "Deleted: all resistors" << endl;
  }
}

void handleSetV(stringstream& ss) {
  int nodeid;
  double volt;
  if (getInteger(ss, nodeid) && getDouble(ss, volt)) {
    if (!nodeInRange(nodeid)) return;
    nodeid--;  // adjust to 0 based
    nodes[nodeid].setVoltage(volt);
    cout << "Set: node " << nodeid + 1 << " to " << fixed << setprecision(2)
         << volt << " Volts" << endl;
  }
}

// Initialize the voltage of all nodes without a specified (setV) voltage to 0.

// while (some nodes voltage has changed by more than MIN_ITERATION_CHANGE) {

//  for (all nodes without a set voltage) {

//    set voltage of node according to Eq. 3

void solve() {
  // Initialize the voltage of all nodes without a specified (setV) voltage to
  // 0.
  for (int i = 0; i < maxNodeNumber; ++i) {
    if (nodes[i].isVoltageSet() == false) {
      nodes[i].setVoltageInternally(0);
    }
  }
  bool iteration;
  do {
    iteration = false;
    for (int i = 0; i < maxNodeNumber; ++i) {
      double sumOfoneoverR = 0;
      double sumofI = 0;
      int* resIdArray = nodes[i].getResIDArray();
      int numResis = nodes[i].getNumRes();
      for (int j = 0; j < numResis;
           ++j) {  // loop over all the resistance on one node
        int rIndex =
            resIdArray[j];  // get the resistor that is connected to this node
        double resis = resistors[rIndex]->getResistance();
        int othernode = resistors[rIndex]->getOtherEndpoint(i);
        sumOfoneoverR += 1 / resis;
        sumofI += nodes[othernode].getVoltage() / resis;
      }
      double newvoltage;
      if (sumOfoneoverR > 0)
        newvoltage = sumofI / sumOfoneoverR;
      else
        newvoltage = 0;
      // check if the change is greater than MIN_ITERATION_CHANGE
      if (abs(newvoltage - nodes[i].getVoltage()) > MIN_ITERATION_CHANGE)
        iteration = true;
      if (!nodes[i].isVoltageSet())  // only change the voltage if it is not set
        nodes[i].setVoltageInternally(newvoltage);
    }
  } while (iteration);
  cout << "Solve:" << endl;
  for (int i = 0; i < maxNodeNumber; ++i) {
    if (nodes[i].getNumRes() > 0) {
      cout << "Node " << i + 1 << ": " << fixed << setprecision(2)
           << nodes[i].getVoltage() << " V" << endl;
    }
  }
}
