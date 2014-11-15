#include "Queryable.h"
using namespace wg::sqlite::base;

Queryable::Queryable()
{
	this->_wheres = new vector<wg_where*>();
}

Queryable::~Queryable()
{
	delete this->_wheres;
}

Queryable* Queryable::where(string field, string eq, string val)
{
	this->_prefix(field);
	wg_where* where = new wg_where();
	where->field = field;
	where->eq = (eq == "" ? "=" : eq); // fallback for morons who pass empty equalizers
	where->value = val;
	this->_wheres->push_back(where);
	return this;
}

Queryable* Queryable::where(string field, string val)
{
	this->_prefix(field);
	wg_where* where = new wg_where();
	where->field = field;
	where->value = val;
	this->_wheres->push_back(where);
	return this;
}

Queryable* Queryable::orWhere(string field, string eq, string val)
{
	this->_prefix(field);
	wg_where* where = new wg_where();
	where->field = field;
	where->value = val;
	where->pref = "OR ";
	this->_wheres->push_back(where);
	return this;
}

Queryable* Queryable::orWhere(string field, string val)
{
	this->_prefix(field);
	wg_where* where = new wg_where();
	where->field = field;
	where->value = val;
	where->pref = "OR ";
	this->_wheres->push_back(where);
	return this;
}

Queryable* Queryable::andWhere(string field, string eq, string val)
{
	this->_prefix(field);
	wg_where* where = new wg_where();
	where->field = field;
	where->value = val;
	where->pref = "AND ";
	this->_wheres->push_back(where);
	return this;
}

Queryable* Queryable::andWhere(string field, string val)
{
	this->_prefix(field);
	wg_where* where = new wg_where();
	where->field = field;
	where->value = val;
	where->pref = "AND ";
	this->_wheres->push_back(where);
	return this;
}