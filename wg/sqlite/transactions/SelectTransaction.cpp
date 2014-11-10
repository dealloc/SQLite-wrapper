#include "SelectTransaction.h"
using namespace wg::sqlite::transactions;


SelectTransaction::SelectTransaction()
{
	this->_selects = new vector<string>();
	this->_tables = new vector<wg_table*>();
	this->_wheres = new vector<wg_where*>();
}


SelectTransaction::~SelectTransaction()
{
	delete this->_selects;
	delete this->_tables;
	delete this->_wheres;
	delete this->_callback;
}

SelectTransaction* SelectTransaction::from(string table)
{
	wg_table* statement = new wg_table();
	statement->name = table;
	this->_tables->push_back(statement);
	this->_current = table;
	return this;
}

SelectTransaction* SelectTransaction::from(string table, string alias)
{
	wg_table* statement = new wg_table();
	statement->name = table;
	statement->alias = alias;
	this->_tables->push_back(statement);
	this->_current = alias;
	return this;
}

SelectTransaction* SelectTransaction::select(string field)
{
	WG_SQLITE_PREFIX(field, this->_current);
	this->_selects->push_back(field);
	return this;
}
SelectTransaction* SelectTransaction::where(string field, string eq, string val)
{
	WG_SQLITE_PREFIX(field, this->_current);
	wg_where* where = new wg_where();
	where->field = field;
	where->eq = (eq == "" ? "=" : eq); // fallback for morons who pass empty equalizers
	where->value = val;
	this->_wheres->push_back(where);
	return this;
}
SelectTransaction* SelectTransaction::where(string field, string val)
{
	WG_SQLITE_PREFIX(field, this->_current);
	wg_where* where = new wg_where();
	where->field = field;
	where->value = val;
	this->_wheres->push_back(where);
	return this;
}

SelectTransaction* SelectTransaction::orWhere(string field, string eq, string val)
{
	WG_SQLITE_PREFIX(field, this->_current);
	wg_where* where = new wg_where();
	where->field = field;
	where->value = val;
	where->pref = "OR ";
	this->_wheres->push_back(where);
	return this;
}
SelectTransaction* SelectTransaction::orWhere(string field, string val)
{
	WG_SQLITE_PREFIX(field, this->_current);
	wg_where* where = new wg_where();
	where->field = field;
	where->value = val;
	where->pref = "OR ";
	this->_wheres->push_back(where);
	return this;
}
SelectTransaction* SelectTransaction::andWhere(string field, string eq, string val)
{
	WG_SQLITE_PREFIX(field, this->_current);
	wg_where* where = new wg_where();
	where->field = field;
	where->value = val;
	where->pref = "AND ";
	this->_wheres->push_back(where);
	return this;
}
SelectTransaction* SelectTransaction::andWhere(string field, string val)
{
	WG_SQLITE_PREFIX(field, this->_current);
	wg_where* where = new wg_where();
	where->field = field;
	where->value = val;
	where->pref = "AND ";
	this->_wheres->push_back(where);
	return this;
}

const string SelectTransaction::build()
{
	// build the SQL query
	stringstream _sql;
	_sql << "SELECT ";
	WG_ITERATE_PTR(it, vector<string>, this->_selects)
	{
		_sql << *it << (WG_LAST_ITER_PTR(it, _selects) ? " " : ", ");
	}
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
	_sql << "FROM ";
	WG_ITERATE_PTR(it, vector<wg_table*>, this->_tables)
	{
		_sql << "[" << (*it)->name << "] AS [" << WG_TABLE_PTR_NAME((*it)) << "]";
		_sql << (WG_LAST_ITER_PTR(it, _tables) ? "" : ", ");
	}
	_sql << ";";
	return _sql.str();
}

bool SelectTransaction::hasCallback()
{
	return (this->_callback == WG_NULL);
}

void SelectTransaction::callback(select_callback &handler)
{
	this->_callback = &handler;
}