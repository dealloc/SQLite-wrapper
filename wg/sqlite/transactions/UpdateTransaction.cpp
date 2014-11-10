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
	delete this->_callback;
}

UpdateTransaction* UpdateTransaction::set(string name, string value)
{
	wg_field* field = new wg_field();
	field->field = name;
	field->value = value;
	this->_fields->push_back(field);
	return this;
}

UpdateTransaction* UpdateTransaction::where(string field, string eq, string val)
{
	wg_where* where = new wg_where();
	where->field = field;
	where->eq = (eq == "" ? "=" : eq); // fallback for morons who pass empty equalizers
	where->value = val;
	this->_wheres->push_back(where);
	return this;
}

UpdateTransaction* UpdateTransaction::where(string field, string val)
{
	wg_where* where = new wg_where();
	where->field = field;
	where->eq = "="; // fallback for morons who pass empty equalizers
	where->value = val;
	this->_wheres->push_back(where);
	return this;
}

UpdateTransaction* UpdateTransaction::orWhere(string field, string eq, string val)
{
	wg_where* where = new wg_where();
	where->field = field;
	where->eq = (eq == "" ? "=" : eq); // fallback for morons who pass empty equalizers
	where->value = val;
	where->pref = "OR";
	this->_wheres->push_back(where);
	return this;
}

UpdateTransaction* UpdateTransaction::orWhere(string field, string val)
{
	wg_where* where = new wg_where();
	where->field = field;
	where->value = val;
	where->eq = "="; // fallback for morons who pass empty equalizers
	where->pref = "OR";
	this->_wheres->push_back(where);
	return this;
}

UpdateTransaction* UpdateTransaction::andWhere(string field, string eq, string val)
{
	wg_where* where = new wg_where();
	where->field = field;
	where->eq = (eq == "" ? "=" : eq); // fallback for morons who pass empty equalizers
	where->value = val;
	where->pref = "AND";
	this->_wheres->push_back(where);
	return this;
}

UpdateTransaction* UpdateTransaction::andWhere(string field, string val)
{
	wg_where* where = new wg_where();
	where->field = field;
	where->eq = "="; // fallback for morons who pass empty equalizers
	where->value = val;
	where->pref = "AND";
	this->_wheres->push_back(where);
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

bool UpdateTransaction::hasCallback()
{
	return (this->_callback == WG_NULL);
}
void UpdateTransaction::callback(update_callback &handler)
{
	this->_callback = &handler;
}