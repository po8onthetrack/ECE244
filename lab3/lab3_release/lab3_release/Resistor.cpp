//
//  Resistor.cpp
//  Lab 3 The Resistor Network Program
// Author: Harry Zhang
// Date: 2025/09/29

#include "Resistor.h"

#include <iomanip>
#include <iostream>
using namespace std;

void Resistor::print() {
  cout << std::left << std::setw(20) << name << std::right << std::setw(8)
       << std::fixed << std::setprecision(2) << resistance << " Ohms "
       << endpointNodeIDs[0] << " -> " << endpointNodeIDs[1] << endl;
}

Resistor::Resistor(string name_, double resistance_, int endpoints_[2]) {
  name = name_;
  resistance = resistance_;
  endpointNodeIDs[2] = endpoints_[2];
}

string Resistor::getName() { return name; }
// returns the name

double Resistor::getResistance() {
  return resistance;
}  // returns the resistance

void Resistor::setResistance(double resistance_) {
  resistance = resistance_;
}  // sets the resistance

int Resistor::getOtherEndpoint(int nodeIndex) {
  if (endpointNodeIDs[0] == nodeIndex)
    return endpointNodeIDs[1];
  else
    return endpointNodeIDs[0];
}  // returns the other node ID that the
   // resistor is attached to (other than nodeIndex)
