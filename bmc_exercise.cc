#include "bmc_exercise.h"

void BMC::alloc_state(BMC::state& st,int n) {
}

IMathSAT::term BMC::init_state(BMC::state& st) {
  return iface.make_false();
}

void BMC::transition(BMC::state& from,BMC::state& to) {
}

void BMC::property(BMC::state& st,std::vector<IMathSAT::term>& vec) {
}

BMC::BMC(int fd,IMathSAT& i) : prog(fd), iface(i) {
}

void BMC::extend() {
}

bool BMC::check() {
  return false;
}

check_result BMC::check(IMathSAT::term* reach,IMathSAT::term* new_reach) {
  return UNIMPLEMENTED;
}
