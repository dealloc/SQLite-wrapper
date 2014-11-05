#include "row.h"
using namespace wg::sqlite::results;

Row::Row()
{
	this->_columns = new map<string, string>();
}

Row::~Row()
{
	delete this->_columns; // cleanup pointers
}

string& Row::operator[](const string column)
{
	return this->_columns->operator[](column);
}