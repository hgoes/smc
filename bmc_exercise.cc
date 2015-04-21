#include "bmc_exercise.h"

void BMC::alloc_state(BMC::state& st,int n) {
}

void BMC::init_state(BMC::state& st) {
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
