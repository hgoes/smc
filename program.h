#pragma once
#include <sexp.h>
#include <map>
#include <string>
#include <vector>

class Program {
private:
  void parse_expression(sexp_t* expr);
public:
  std::map<std::string,sexp_t*> vars;
  std::vector<sexp_t*> inits;
  std::vector<sexp_t*> trans;
  std::vector<sexp_t*> props;
  Program(sexp_t* expr);
  Program(int fd);
};
