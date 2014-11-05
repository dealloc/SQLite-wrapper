#include "InsertTransaction.h"
using namespace wg::sqlite::transactions;


InsertTransaction::InsertTransaction(const char* name) : _name(name)
{
	this->_fields = new vector<wg_field*>();
}

InsertTransaction::InsertTransaction(string name) : _name(name)
{
	this->_fields = new vector<wg_field*>();
}


InsertTransaction::~InsertTransaction()
{
	delete this->_fields;
}


InsertTransaction* InsertTransaction::insert(string field, string value)
{
	wg_field* insert = new wg_field();
	insert->field = field;
	insert->value = value;
	this->_fields->push_back(insert);
	return this;
}

InsertTransaction* InsertTransaction::insert(string field, int value)
{
	wg_field* insert = new wg_field();
	insert->field = field;
	insert->value = WG_ISTR(value); // TODO convert to  correct string
	this->_fields->push_back(insert);
	return this;
}

const string InsertTransaction::build()
{
	stringstream _sql;
	_sql << "INSERT INTO " << this->_name << " (";
	WG_ITERATE_PTR(it, vector<wg_field*>, this->_fields)
		_sql << "[" << (*it)->field << "]" << (WG_LAST_ITER_PTR(it, this->_fields) ? "" : ", ");
	_sql << ") VALUES (";
	WG_ITERATE_PTR(it, vector<wg_field*>, this->_fields)
		_sql << "'" << (*it)->value << "'" << (WG_LAST_ITER_PTR(it, this->_fields) ? "" : ", ");
	_sql << ");";
	return _sql.str();
}