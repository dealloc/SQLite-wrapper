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
			template<typename T> class Callable
			{
			public:
				static bool hasCallback() { return Callable<T>::_callback == WG_NULL; }
				static void callback(T handler) { Callable<T>::_callback = handler; }
				static T getCallback() { return Callable<T>::_callback; }
			protected:
				static T _callback;
			};
			template<typename T> T Callable<T>::_callback;
		}
	}
}

#endif