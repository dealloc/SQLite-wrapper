#ifndef WG_CALLABLE_H
#define WG_CALLABLE_H

#include "../../wg_utils.h"
#include "../structs.h"

namespace wg
{
	namespace sqlite
	{
		namespace base
		{
			/*
			* SOLUTION TO THE RECURRING VARIABLE PROBLEM:
			* both update and insert have the same typedef callback (just with a different name)
			* thus internally they use the same class and as such they use the same variable
			* SOLUTION: change the function prototype of either one so signatures no longer match
			*/
			template<typename T> class Callable
			{
			public:
				static bool hasCallback() { return Callable<T>::_callback == WG_NULL; }
				static void callback(T handler) { Callable<T>::_callback = handler; }
				static T getCallback() { return Callable<T>::_callback; }
			protected:
				static T _callback;
			};
			template<typename T> T Callable<T>::_callback; // force allocation of space for static
		}
	}
}

#endif