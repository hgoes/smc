#pragma once
#include "program.h"
#include "parser.h"
#include "mathsat_interface.h"

#include <map>
#include <string>
#include <vector>

class BMC {
private:
  Program prog;
  IMathSAT& iface;

  // A state is a mapping from names to declarations
  typedef std::map<std::string,IMathSAT::decl> state;

  // All the allocated states.
  // states[0] is the initial state.
  std::vector<state> states;
  
  // These are helper function that could be useful in your implementation, but
  // you are free to create your own.
  
  // Create fresh variables for a new state
  void alloc_state(state& st,int n);
  // Assert that the state is the initial state
  void init_state(state& st);
  // Create a transition between two states
  void transition(state& from,state& to);
  // Get a vector containing all properties of a given state
  void property(state& st,std::vector<IMathSAT::term>& vec);
public:
  // Constructor
  BMC(int fd,IMathSAT& i);
  
  // These are the main two functions that you have to implement:

  // Extends the size of the BMC instance by one. Creates a new state, asserts
  // that there is a transition from the previous state to the new one.
  void extend();

  // Check if the current BMC instance is satisfiable (i.e. the program contains a bug).
  bool check();
};
