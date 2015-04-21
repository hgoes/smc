#include "program.h"
#include <cstring>
#include <iostream>

void Program::parse_expression(sexp_t* expr) {
  if(expr==NULL) {
    throw std::string("Invalid S-expression.");
  }
  if(expr->ty!=SEXP_LIST) {
    throw std::string("Must be a list.");
  }
  if(expr->list==NULL || expr->list->ty!=SEXP_VALUE || strcmp("program",expr->list->val)!=0) {
    throw std::string("Must have \"program\" as first item.");
  }
  sexp_t* item = expr->list->next;
  while(item!=NULL) {
    if(item->ty!=SEXP_LIST) {
      throw std::string("All sub-items of program must be lists.");
    }
    if(item->list->ty!=SEXP_VALUE) {
      throw std::string("All sub-items of program must be lists starting with a name.");
    }
    if(strcmp("state",item->list->val)==0) {
      sexp_t* cur_state = item->list->next;
      while(cur_state!=NULL) {
	if(cur_state->ty!=SEXP_LIST) {
	  throw std::string("State variable must be a list");
	}
	if(cur_state->list->ty!=SEXP_VALUE) {
	  throw std::string("State variable must start with a name");
	}
	vars.insert(std::map<std::string,sexp_t*>::value_type(cur_state->list->val,
							      cur_state->list->next));
	cur_state = cur_state->next;
      }
    } else if(strcmp("init",item->list->val)==0) {
      sexp_t* cur_init = item->list->next;
      while(cur_init!=NULL) {
	inits.push_back(cur_init);
	cur_init = cur_init->next;
      }
    } else if(strcmp("transition",item->list->val)==0) {
      sexp_t* cur_trans = item->list->next;
      while(cur_trans!=NULL) {
	trans.push_back(cur_trans);
	cur_trans = cur_trans->next;
      }
    } else if(strcmp("property",item->list->val)==0) {
      sexp_t* cur_prop = item->list->next;
      while(cur_prop!=NULL) {
	props.push_back(cur_prop);
	cur_prop = cur_prop->next;
      }
    } else {
      throw std::string("Unknown item in program: ")+item->list->val;
    }
    item = item->next;
  }
}

Program::Program(sexp_t* expr) {
  parse_expression(expr);
}

Program::Program(int fd) {
  sexp_iowrap_t* wrap = init_iowrap(fd);
  sexp_t* program = read_one_sexp(wrap);
  parse_expression(program);
  destroy_iowrap(wrap);
}
