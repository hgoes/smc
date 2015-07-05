#pragma once
#include "program.h"
#include "parser.h"
#include "mathsat_interface.h"

#include <map>
#include <vector>
#include <unordered_set>
#include <iostream>

// Helper function to print states
template<typename T>
std::ostream& operator<<(std::ostream& s, const std::vector<T>& v) {
    s.put('[');
    char comma[3] = {'\0', ' ', '\0'};
    for (const auto& e : v) {
        s << comma << e;
        comma[0] = ',';
    }
    return s << ']';
}

class PredAbs {
  Program prog;
  IMathSAT& iface;
  // The concrete state
  typedef std::map<std::string,IMathSAT::decl> state;
  // Each boolean in this vector denotes whether the predicate is true or false in the state
  typedef std::vector<bool> abs_state;
  // The database contains all visited states
  typedef std::unordered_set<abs_state> state_db;

  // We need two concrete states to perform transition abstraction
  state from;
  state to;
  
  state_db reachable;

  // A list of states that we have to calculate successors to
  std::vector<abs_state> todo;
  
  // Create fresh variables for a new state
  void alloc_state(state& st, int n);

  // Assert that a state is the initial state
  void init_state(state& st);

  // Assert the transition relation between two states
  void transition(state& from,state& to);

  // Assert the negated property
  void neg_property(state& st);

  // Add a new abstract state to the database
  // If it has not been in the database, it must go into the todo list
  void add_state(abs_state& st);

  // Assert that a state is a given abstract state
  void assert_abstract(state& st, abs_state& abs);

  // Extract all abstract states from a state and add them to the database
  void extract_states(state& st);

  // Add the initial states to the database
  void add_init_states();

  // Check if an abstract state contains an error state
  bool check_abstract_state(abs_state& st);

  // Add all the abstract states reachable from a given reachable state
  void add_transition_states(abs_state& src);

public:
  PredAbs(int fd,IMathSAT& i);
  
  // Check the abstract model
  bool check();
};
