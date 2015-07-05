#include "pred_abs_exercise.h"

void PredAbs::alloc_state(state& st, int n) {
  SMTLibParser<IMathSAT> parser(iface);
  for(auto it = prog.vars.begin() ; it!=prog.vars.end() ; it++) {
    const std::string& name = it->first;
    sexp_t* tp = it->second;
    IMathSAT::type rtp = parser.parse_type(tp);
    std::ostringstream conv;
    conv << name << "_" << n;
    IMathSAT::decl var = iface.make_decl(conv.str(),rtp);
    st.insert(state::value_type(name,var));
  }
}

void PredAbs::init_state(state& st) {
  SMTLibParser<IMathSAT> parser(iface);
  for(auto it = st.begin(); it!=st.end(); it++) {
    parser.add_named_term(it->first,it->second);
  }
  for(auto it = prog.inits.begin(); it!=prog.inits.end(); it++) {
    IMathSAT::term t = parser.parse_term(*it);
    iface.assert(t);
  }
}

void PredAbs::transition(state& from,state& to) {
  SMTLibParser<IMathSAT> parser(iface);
  for(auto it = from.begin() ; it!=from.end() ; it++) {
    parser.add_named_term(it->first,it->second);
  }
  for(auto it = to.begin() ; it!=to.end() ; it++) {
    parser.add_named_term(it->first+"_",it->second);
  }
  for(auto it = prog.trans.begin() ; it!=prog.trans.end() ; it++) {
    IMathSAT::term t = parser.parse_term(*it);
    iface.assert(t);
  }
}

void PredAbs::neg_property(state& st) {
  std::vector<IMathSAT::term> vec;
  SMTLibParser<IMathSAT> parser(iface);
  for(auto it = st.begin() ; it!=st.end() ; it++) {
    parser.add_named_term(it->first,it->second);
  }
  for(auto it = prog.props.begin() ; it!=prog.props.end() ; it++) {
    IMathSAT::term t = parser.parse_term(*it);
    vec.push_back(iface.make_not(t));
  }
  iface.assert(iface.make_or(vec));
}

void PredAbs::add_state(abs_state& st) {
  // TODO
}

void PredAbs::assert_abstract(state& st, abs_state& abs) {
  // TODO
}

void PredAbs::extract_states(state& st) {
  // TODO
}

void PredAbs::add_init_states() {
  // TODO
}

bool PredAbs::check_abstract_state(abs_state& st) {
  // TODO
  return false;
}

void PredAbs::add_transition_states(abs_state& src) {
  // TODO
}

PredAbs::PredAbs(int fd,IMathSAT& i) : prog(fd), iface(i) {
  alloc_state(from,0);
  alloc_state(to,1);
}

bool PredAbs::check() {
  add_init_states();
  while(!todo.empty()) {
    abs_state cur = todo.back();
    todo.pop_back();
    if(!check_abstract_state(cur)) {
      return false;
    }
    add_transition_states(cur);
  }
  return true;
}
