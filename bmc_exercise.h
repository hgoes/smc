#pragma once
#include "program.h"
#include "parser.h"
#include "mathsat_interface.h"

#include <map>
#include <string>
#include <vector>

// The result of the interpolating BMC procedure:
enum check_result {
  COMPLETE,   // A fixpoint has been found so we know the BMC instance covers all reachable states
  INCOMPLETE, // No bug was found, but the instance doesn't cover all reachable states
  NEW_APPROX,  // A new approximation of the set of reachable states has been found
  UNIMPLEMENTED // The function hasn't been implemented yet
};

class BMC {
private:
  Program prog;
  IMathSAT& iface;

  // A state is a mapping from names to declarations
  typedef std::map<std::string,IMathSAT::decl> state;

  // A mapping that stores which name belongs to which variable in a state
  typedef std::map<IMathSAT::decl,std::string> rev_state;
  
  // All the allocated states.
  // states[0] is the initial state.
  std::vector<state> states;

  // A reverse mapping for state 1
  rev_state rev_state1;

  // Interpolation Groups A and B
  IMathSAT::interp_group group_A,group_B;
  
  // These are helper function that could be useful in your implementation, but
  // you are free to create your own.
  
  // Create fresh variables for a new state
  void alloc_state(state& st,int n);
  // Creates an expression that represents the fact that the state is initial
  IMathSAT::term init_state(state& st);
  // Create a transition between two states
  void transition(state& from,state& to);
  // Get a vector containing all properties of a given state
  void property(state& st,std::vector<IMathSAT::term>& vec);
  // Check if a term implies another one
  bool check_implication(IMathSAT::term x,IMathSAT::term y);
public:
  // Constructor
  BMC(int fd,IMathSAT& i);
  
  // These are the main two functions that you have to implement:

  // Extends the size of the BMC instance by one. Creates a new state, asserts
  // that there is a transition from the previous state to the new one.
  void extend();

  // Check if the current BMC instance is satisfiable (i.e. the program contains a bug).
  bool check();

  // Check if we can reach a bad state from reach.
  // If reach is NULL, it is assumed to be the initial state.
  // If a better approximation of the reachable states is found, store it in new_reach and
  // return NEW_APPROX.
  check_result check(IMathSAT::term* reach,IMathSAT::term* new_reach);
};
