#pragma once
#include <sexp.h>
#include <map>
#include <vector>
#include <cstring>
#include <ctype.h>
#include <sstream>

template <typename SMTLibEnv> class SMTLibParser {
private:
  SMTLibEnv& env;
  std::map<std::string,typename SMTLibEnv::decl> named_terms;
  typename SMTLibEnv::term parse_atom(const char* name) {
    if(isdigit(name[0])) {
      std::string rname(name);
      return env.make_number(rname);
    } else {
      std::string term_name(name);
      auto entry = named_terms.find(term_name);
      if(entry != named_terms.end()) {
	return env.make_var(entry->second);
      } else {
	throw std::string("Cannot find variable ")+name;
      }
    }
  }
  inline void argument_check(const std::string& fname,int req,const std::vector<typename SMTLibEnv::term>& args) const {
    if(args.size()!=req) {
      std::ostringstream conv;
      conv << "Wrong number of arguments to '"
	   << fname << "' function: "
	   << req << " required, got "
	   << args.size() << ".";
      throw conv.str();
    }
  }
public:
  SMTLibParser(SMTLibEnv& e) : env(e) {
  }
  void add_named_term(const std::string& name,typename SMTLibEnv::decl term) {
    named_terms.insert(typename std::map<std::string,typename SMTLibEnv::decl>::value_type(name,term));
  }
  typename SMTLibEnv::term parse_term(sexp_t* t) {
    switch(t->ty) {
    case SEXP_VALUE:
      return parse_atom(t->val);
    case SEXP_LIST:
      if(t->list==NULL) {
	throw std::string("Empty list is not a valid SMT expression.");
      } else {
	sexp_t* head = t->list;
	std::vector<typename SMTLibEnv::term> args;
	sexp_t* cur = head->next;
	while(cur!=NULL) {
	  typename SMTLibEnv::term parsed = parse_term(cur);
	  args.push_back(parsed);
	  cur = cur->next;
	}
	switch(head->ty) {
	case SEXP_VALUE:
	  if(strcmp("and",head->val)==0) {
	    return env.make_and(args);
	  }
	  if(strcmp("or",head->val)==0) {
	    return env.make_or(args);
	  }
	  if(strcmp("=>",head->val)==0) {
	    return env.make_implication(args);
	  }
	  if(strcmp("not",head->val)==0) {
	    argument_check("not",1,args);
	    return env.make_not(args[0]);
	  }
	  if(strcmp("<",head->val)==0) {
	    argument_check("<",2,args);
	    return env.make_lt(args[0],args[1]);
	  }
	  if(strcmp("<=",head->val)==0) {
	    argument_check("<=",2,args);
	    return env.make_leq(args[0],args[1]);
	  }
	  if(strcmp(">",head->val)==0) {
	    argument_check(">",2,args);
	    return env.make_gt(args[0],args[1]);
	  }
	  if(strcmp(">=",head->val)==0) {
	    argument_check(">=",2,args);
	    return env.make_geq(args[0],args[1]);
	  }
	  if(strcmp("=",head->val)==0) {
	    return env.make_eq(args);
	  }
	  if(strcmp("+",head->val)==0) {
	    return env.make_plus(args);
	  }
	  if(strcmp("*",head->val)==0) {
	    return env.make_times(args[0],args[1]);
	  }
	  if(strcmp("ite",head->val)==0) {
	    argument_check("ite",3,args);
	    return env.make_ite(args[0],args[1],args[2]);
	  }
	  throw std::string("Unknown function ")+head->val;
	}
      }
    }
  }
  typename SMTLibEnv::type parse_type(sexp_t* t) {
    switch(t->ty) {
    case SEXP_VALUE:
      if(strcmp("Bool",t->val)==0) {
	return env.bool_type();
      }
      if(strcmp("Int",t->val)==0) {
	return env.int_type();
      }
    }
  }
};
