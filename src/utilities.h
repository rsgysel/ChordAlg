#ifndef CHORDALG_UTILITIES_H
#define CHORDALG_UTILITIES_H

#include <algorithm>
#include <iostream>
#include <vector>

namespace chordalg {

// Debugging Macros
#define DEBUG_ENTER\
      std::cout << "DEBUG: ENTERING " << __PRETTY_FUNCTION__ << std::endl << std::endl;

#define DEBUG_EXIT\
      std::cout << "DEBUG: EXITING " << __PRETTY_FUNCTION__ << std::endl << std::endl;

#define DEBUG_LINE\
      std::cout << "DEBUG: AT LINE " << __LINE__ << " of " << __PRETTY_FUNCTION__ << std::endl << std::endl;

#define DEBUG_DATA(data)\
      std::cout << "DEBUG: DATA "  << data << " at LINE " << __LINE__ << " of " << __PRETTY_FUNCTION__ << std::endl << std::endl;

#define DEBUG_VARIABLE(description, data)\
      std::cout << "DEBUG: DATA "  << data << " for VARIABLE " << description << " at LINE " << __LINE__ << " of " << __PRETTY_FUNCTION__ << std::endl << std::endl;

#define DEBUG_RANGE(description, data, type)\
      std::cout << "DEBUG: DATA ";\
      std::copy( data.begin(), data.end(), std::ostream_iterator< type >(std::cout, " ") );\
      std::cout << std::endl << "for RANGE " << description << " at LINE " << __LINE__ << " of " << __PRETTY_FUNCTION__ << std::endl << std::endl;
} // namespace chordalg

#endif // CHORDALG_UTILITIES_H

