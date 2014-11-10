#ifndef WG_UTILS_H
#define WG_UTILS_H

#include <iostream>
#include <stdlib.h>

#ifndef WG_DEBUG
// if set to 1, debugging macros will be enabled
#define WG_DEBUG 1
#endif

// all debug only macro definitions should come here
#if WG_DEBUG == 1

// the log macro
#define WG_LOG(message) std::cout << "[" << __FILE__ << ":" << __LINE__ << "]: " << message << std::endl;

#else

// the log macro (deactivated)
#define WG_LOG(message)

#endif

// check for C++11 support
#if __cplusplus > 199711L || _MSC_VER == 1800
// C++11 is enabled!
#define WG_Cpp11
#endif

#ifdef WG_Cpp11
#define WG_NULL nullptr
#else
#define WG_NULL NULL
#endif

// create an iterator with a name for a type
#define WG_ITER(name, type) type::iterator name;

#define WG_TYPE_ITER(type, name) typedef type::iterator name;

// iterate over an iterable STL container pointer with name as the iterator, type as the type of container and cont as container
#define WG_ITERATE_PTR(name, type, cont) for(type::iterator name = cont->begin(); name != cont->end(); name++)

// iterate over an iterable STL container with name as the iterator, type as the type of container and cont as container
#define WG_ITERATE(name, type, cont) for(type::iterator name = cont.begin(); name != cont.end(); name++)

// is true if this is the last element of an iteration
#define WG_LAST_ITER(it, cont) (it == cont.end()-1)

// is true if this is the last element of an iteration
#define WG_LAST_ITER_PTR(it, cont) (it == cont->end()-1)

// include std::string in the local scope (can also be changed to typedef string
#define WG_USE_STRING using std::string

// import the given type into the current scope (imported from STD)
#define WG_USE(type) using std::type

// the wildcard for SQLite queries
#define WG_SQLITE_WILDCARD "*"

// prefix an sql field with the given table name if the field isn't prefixed already
#define WG_SQLITE_PREFIX(field, pref) if(pref != "" && field.find(".") == std::string::npos) field = pref+"."+field;

// check if a std::string is numeric
#define WG_STR_ISNUM(var) (atoi(var.c_str()) != 0 && var != "0")

#define WG_CH_ISNUM(var) (atoi(var) != 0 && var != "0")

// convert int to string using sstream
#define WG_ISTR( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

#endif
