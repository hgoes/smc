#include "bmc_exercise.h"
#include "pred_abs_exercise.h"
#include "mathsat_interface.h"
#include <iostream>
#include <unistd.h>
#include <cstdlib>

enum operation_mode {
  OP_MODE_BMC,
  OP_MODE_PRED_ABS
};

int main(int argc,char* argv[]) {
  try {
    if(argc < 2) {
      std::cerr << "Please specify mode of operation (either bmc or pred_abs)." << std::endl;
      return -1;
    }
    operation_mode mode;
    int limit;
    if(strcmp(argv[1],"bmc")==0) {
      mode = OP_MODE_BMC;
      if(argc < 3) {
	std::cerr << "Please specify an unwinding limit for bounded model checking." << std::endl;
	return -1;
      } else {
	limit = atoi(argv[2]);
      }
    } else if(strcmp(argv[1],"pred_abs")==0) {
      mode = OP_MODE_PRED_ABS;
    } else {
      mode = OP_MODE_BMC;
      limit = atoi(argv[1]);
    }
    switch(mode) {
    case OP_MODE_BMC:
      {
	IMathSAT iface("QF_AUFLIA");
	BMC bmc(STDIN_FILENO,iface);
	for(int i = 0; i < limit; i++) {
	  std::cout << "Level " << i << "..." << std::endl;
	  bmc.extend();
	  bool res = bmc.check();
	  if(res) {
	    std::cout << "Error found." << std::endl;
	    return 1;
	  }
	}
	IMathSAT::term reach;
	IMathSAT::term new_reach;
	bmc.extend();
	if(bmc.check(nullptr,&reach)==INCOMPLETE) {
	  std::cout << "Error found." << std::endl;
	  return 1;
	}
	std::cout << "No error found." << std::endl;
	while(true) {
	  switch(bmc.check(&reach,&new_reach)) {
	  case INCOMPLETE:
	    std::cout << "(Not complete)" << std::endl;
	    return 2;
	  case COMPLETE:
	    std::cout << "(Complete)" << std::endl;
	    return 0;
	  default:
	    reach = new_reach;
	    break;
	  }
	}
	return 0;
      }
    case OP_MODE_PRED_ABS:
      {
	IMathSAT iface("QF_AUFLIA", false);
	PredAbs pred_abs(STDIN_FILENO,iface);
	if(pred_abs.check()) {
	  std::cout << "Model has no errors." << std::endl;
	} else {
	  std::cout << "Possible error found." << std::endl;
	}
	return 0;
      }
    }
  } catch(std::string msg) {
    std::cerr << "Internal error: " << msg << std::endl;
    return -1;
  }
}
