#ifndef WG_BUILDABLE_H
#define WG_BUILDABLE_H

#include <string>
#include "../../wg_utils.h"

namespace wg
{
	namespace sqlite
	{
		namespace base
		{
			WG_USE_STRING;

			class Buildable
			{
				virtual const string build() = 0;
			};
		}
	}
}

#endif