#include "UpdateTransaction.h"
using namespace wg::sqlite::transactions;


UpdateTransaction::UpdateTransaction(const char* name) : _name(name)
{
	this->_fields = new vector<wg_field*>();
	this->_wheres = new vector<wg_where*>();
}

UpdateTransaction::UpdateTransaction(const string name) : _name(name)
{
	this->_fields = new vector<wg_field*>();
	this->_wheres = new vector<wg_where*>();
}


UpdateTransaction::~UpdateTransaction()
{
	delete this->_fields;
	delete this->_wheres;
}

UpdateTransaction* UpdateTransaction::set(string name, string value)
{
	wg_field* field = new wg_field();
	field->field = name;
	field->value = value;
	this->_fields->push_back(field);
	return this;
}

const string UpdateTransaction::build()
{
	stringstream _sql;
	_sql << "UPDATE " << this->_name << " SET";
	WG_ITERATE_PTR(it, vector<wg_field*>, this->_fields)
		_sql << " [" << (*it)->field << "] = '" << (*it)->value << "'" << (WG_LAST_ITER_PTR(it, this->_fields) ? "" : ",");

	if (this->_wheres->size() != 0)
	{
		_sql << " WHERE (";
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

inline void UpdateTransaction::_prefix(string &field)
{
	// prefixing in an update statement causes errors
}