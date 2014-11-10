#include "CreateTransaction.h"
using namespace wg::sqlite::transactions;


CreateTransaction::CreateTransaction(const char* table) : _name(table)
{
	this->_columns = new vector<Column*>();
}

CreateTransaction::CreateTransaction(string table) : _name(table)
{
	this->_columns = new vector<Column*>();
}

CreateTransaction::~CreateTransaction()
{
	delete this->_columns;
	delete this->_callback;
}

CreateTransaction* CreateTransaction::gaurd()
{
	this->_gaurd = true;
	return this;
}

Column* CreateTransaction::varchar(string field)
{
	Column* column = new Column(field, WG_SQLITE_VARCHAR);
	this->_columns->push_back(column);
	return column;
}
Column* CreateTransaction::integer(string field)
{
	Column* column = new Column(field, WG_SQLITE_INTEGER);
	this->_columns->push_back(column);
	return column;
}
Column* CreateTransaction::boolean(string field)
{
	Column* column = new Column(field, WG_SQLITE_BOOLEAN);
	this->_columns->push_back(column);
	return column;
}

const string CreateTransaction::build()
{
	stringstream _sql;
	_sql << "CREATE TABLE" << (this->_gaurd ? " IF NOT EXISTS" : "") << " `" << this->_name << "` (";
	WG_ITERATE_PTR(it, vector<Column*>, this->_columns)
	{
		_sql << "[" << (*it)->getName() << "] " << (*it)->getType();
		if ((*it)->getRequired())
			_sql << " NOT NULL";
		if ((*it)->getDefault() != "")
			_sql << " DEFAULT '" << (*it)->getDefault() << "'";
		if ((*it)->getPrimary())
			_sql << " PRIMARY KEY";
		if ((*it)->getIncrements())
			_sql << " AUTOINCREMENT";
		if ((*it)->getUnique())
			_sql << " UNIQUE";
		if (!WG_LAST_ITER_PTR(it, this->_columns))
			_sql << ", ";
	}
	_sql << ");";
	return _sql.str();
}

bool CreateTransaction::hasCallback()
{
	return (this->_callback == WG_NULL);
}

void CreateTransaction::callback(create_callback &callback)
{
	this->_callback = &callback;
}