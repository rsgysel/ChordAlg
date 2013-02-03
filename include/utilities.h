#ifndef CHORDALD_UTILITIES_H
#define CHORDALG_UTILITIES_H

#include <vector>

namespace chordalg {

// Macros

    // A macro to disallow the copy constructor and operator= functions
    // This should be used in the private: declarations for a class
    // Taken from Google's Style Guide: http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml
    #define DISALLOW_COPY_AND_ASSIGN(TypeName) \
      TypeName(const TypeName&);               \
      void operator=(const TypeName&)

    // A macro to disallow default construction
    // This should be used in the private: declarations for a class
    #define DISALLOW_DEFAULT_CONSTRUCTOR(TypeName) \
      TypeName();

} // namespace chordalg

#endif // CHORDALG_UTILITIES_H

