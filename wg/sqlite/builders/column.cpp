#include "column.h"
using namespace wg::sqlite::builders;


Column::Column(const char* name, const char* type) : _name(name), _type(type)
{
	this->_default = "";
	this->_increments = false;
	this->_notnull = false;
	this->_primary = false;
	this->_unique = false;
}

Column::Column(const string name, const string type) : _name(name), _type(type)
{
	this->_default = "";
	this->_increments = false;
	this->_notnull = false;
	this->_primary = false;
	this->_unique = false;
}

const string Column::getName()
{
	return this->_name;
}

const string Column::getType()
{
	return this->_type;
}

Column* Column::primary()
{
	this->_primary = true;
	return this;
}

Column* Column::increments()
{
	this->_increments = true;
	return this;
}

Column* Column::required()
{
	this->_notnull = true;
	return this;
}

Column* Column::unique()
{
	this->_unique = true;
	return this;
}

Column* Column::ifNull(string name)
{
	this->_default = name;
	return this;
}

bool Column::getPrimary()
{
	return _primary;
}
bool Column::getIncrements()
{
	return _increments;
}
bool Column::getRequired()
{
	return _notnull;
}
bool Column::getUnique()
{
	return _unique;
}
string Column::getDefault()
{
	return _default;
}

const string Column::build()
{
	stringstream _sql;
	_sql << "`" << this->_name << "` " << this->_type;
	if (this->_primary)
		_sql << " PRIMARY KEY";
	if (this->_increments)
		_sql << " AUTOINCREMENT";
	if (this->_notnull)
		_sql << " NOT NULL";
	if (this->_unique)
		_sql << " UNIQUE";
	if (this->_default != "")
		_sql << " DEFAULT '" << this->_default << "'";
	return _sql.str();
}
