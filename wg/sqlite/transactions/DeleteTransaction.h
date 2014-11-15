#ifndef WG_DELETETRANSACTION_H
#define WG_DELETETRANSACTION_H

#include <string>
#include "../../wg_utils.h"
#include "../structs.h"

namespace wg
{
	namespace sqlite
	{
		namespace transactions
		{
			WG_USE(string);

			class DeleteTransaction
			{
			public:
				DeleteTransaction(const char* table);
				DeleteTransaction(const string table);
				~DeleteTransaction();
			};
		}
	}
}

#endif