
#ifndef INITIALIZER_H
#define INITIALIZER_H

#include <list>
#include <string>
#include "StateRepresentation.h"
#include <vector>

namespace Initializer{

  std::list<std::string> readInput();
  StateRepresentation storeInput();
  StateRepresentation setupWorld();
}



#endif
