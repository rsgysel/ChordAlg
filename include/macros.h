#ifndef MACROS_H_INCLUDED
#define MACROS_H_INCLUDED

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

#endif // MACROS_H_INCLUDED
