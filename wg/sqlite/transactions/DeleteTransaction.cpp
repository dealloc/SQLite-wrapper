#include "DeleteTransaction.h"
using namespace wg::sqlite::transactions;

DeleteTransaction::DeleteTransaction(const string table) : _name(table)
{
}

DeleteTransaction::DeleteTransaction(const char* table) : _name(table)
{
}

DeleteTransaction::~DeleteTransaction()
{
}

const string DeleteTransaction::build()
{
	stringstream _sql;
	_sql << "DELETE FROM ";
	_sql << "[" << this->_name << "] ";
	if (this->_wheres->size() != 0)
	{
		_sql << "WHERE (";
		WG_ITERATE_PTR(it, vector<wg_where*>, this->_wheres)
		{
			_sql << (*it)->pref << "("
				<< "[" << (*it)->field << "] " << (*it)->eq << " "
				<< (WG_STR_ISNUM((*it)->value) ? "" : "'") << (*it)->value
				<< (WG_STR_ISNUM((*it)->value) ? "" : "'") << (WG_LAST_ITER_PTR(it, _wheres) ? ")" : ") ");
		}
		_sql << ") ";
	}
	_sql << ";";
	return _sql.str();
}

inline void DeleteTransaction::_prefix(string &field)
{
	//WG_SQLITE_PREFIX(field, this->_name); // prefixing causes invalid delete statements
}