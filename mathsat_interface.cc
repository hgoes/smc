#include "mathsat_interface.h"

IMathSAT::IMathSAT(const char* logic) {
  cfg = msat_create_default_config(logic);
  env = msat_create_env(cfg);
}

IMathSAT::~IMathSAT() {
  msat_destroy_env(env);
  msat_destroy_config(cfg);
}

IMathSAT::term IMathSAT::error_term() {
  msat_term res;
  MSAT_MAKE_ERROR_TERM(res);
  return res;
}

bool IMathSAT::is_error_term(IMathSAT::term t) {
  return MSAT_ERROR_TERM(t);
}

IMathSAT::term IMathSAT::make_eq(const std::vector<IMathSAT::term>& args) {
  switch(args.size()) {
  case 0:
  case 1:
    return msat_make_true(env);
  case 2:
    return msat_make_equal(env,args[0],args[1]);
  default:
    IMathSAT::term res = msat_make_equal(env,args[0],args[1]);
    for(int i=2; i<args.size() ; i++) {
      res = msat_make_and(env,res,msat_make_equal(env,args[0],args[i]));
    }
    return res;
  }
}

IMathSAT::term IMathSAT::make_false() {
  return msat_make_false(env);
}

IMathSAT::term IMathSAT::make_true() {
  return msat_make_true(env);
}

IMathSAT::term IMathSAT::make_and(const std::vector<IMathSAT::term>& args) {
  auto it = args.begin();
  if(it==args.end()) {
    return msat_make_true(env);
  }
  IMathSAT::term res = *it;
  for(it++; it!=args.end(); it++) {
    res = msat_make_and(env,res,*it);
  }
  return res;
}

IMathSAT::term IMathSAT::make_or(const std::vector<IMathSAT::term>& args) {
  auto it = args.begin();
  if(it==args.end()) {
    return msat_make_false(env);
  }
  IMathSAT::term res = *it;
  for(it++; it!=args.end(); it++) {
    res = msat_make_or(env,res,*it);
  }
  return res;
}

IMathSAT::term IMathSAT::make_implication(const std::vector<IMathSAT::term>& args) {
  auto it = args.begin();
  if(it==args.end()) {
    return msat_make_true(env);
  }
  IMathSAT::term res = *it;
  for(it++; it!=args.end(); it++) {
    res = msat_make_or(env,msat_make_not(env,res),*it);
  }
  return res;
}

IMathSAT::term IMathSAT::make_not(IMathSAT::term t) {
  return msat_make_not(env,t);
}

IMathSAT::decl IMathSAT::make_decl(const std::string& name,IMathSAT::type tp) {
  return msat_declare_function(env,name.c_str(),tp);
}

IMathSAT::term IMathSAT::make_var(IMathSAT::decl d) {
  IMathSAT::term res = msat_make_constant(env,d);
  if(MSAT_ERROR_TERM(res)) {
    throw std::string("Failed to create variable from declaration.");
  }
  return res;
}

IMathSAT::term IMathSAT::make_number(std::string& str) {
  return msat_make_number(env,str.c_str());
}

IMathSAT::term IMathSAT::make_lt(IMathSAT::term t1,IMathSAT::term t2) {
  return msat_make_not(env,msat_make_leq(env,t2,t1));
}

IMathSAT::term IMathSAT::make_leq(IMathSAT::term t1,IMathSAT::term t2) {
  return msat_make_leq(env,t1,t2);
}

IMathSAT::term IMathSAT::make_gt(IMathSAT::term t1,IMathSAT::term t2) {
  return msat_make_not(env,msat_make_leq(env,t1,t2));
}

IMathSAT::term IMathSAT::make_geq(IMathSAT::term t1,IMathSAT::term t2) {
  return msat_make_leq(env,t2,t1);
}

IMathSAT::term IMathSAT::make_plus(const std::vector<IMathSAT::term>& args) {
  switch(args.size()) {
  case 0:
    return msat_make_number(env,"0");
  case 1:
    return args[0];
  default:
    auto it = args.begin();
    IMathSAT::term res = *it;
    for(it++; it!=args.end(); it++) {
      res = msat_make_plus(env,res,*it);
    }
    return res;
  }
}

IMathSAT::term IMathSAT::make_ite(IMathSAT::term c,IMathSAT::term t,IMathSAT::term f) {
  return msat_make_term_ite(env,c,t,f);
}

IMathSAT::type IMathSAT::bool_type() {
  return msat_get_bool_type(env);
}

IMathSAT::type IMathSAT::int_type() {
  return msat_get_integer_type(env);
}

std::string IMathSAT::term_to_string(IMathSAT::term t) {
  char* res = msat_term_repr(t);
  std::string str_res(res);
  msat_free(res);
  return str_res;
}

void IMathSAT::assert(IMathSAT::term t) {
  msat_assert_formula(env,t);
}

void IMathSAT::push() {
  msat_push_backtrack_point(env);
}

void IMathSAT::pop() {
  msat_pop_backtrack_point(env);
}

bool IMathSAT::check_sat() {
  switch(msat_solve(env)) {
  case MSAT_SAT:
    return true;
  case MSAT_UNSAT:
    return false;
  default:
    throw std::string("Unknown SAT query result.");
  }
}
