#include "database.h"
using namespace wg::sqlite;

Database::Database(const char* name)
{
	sqlite3_open(name, &this->_db); // TODO: check return for SQLITE_OK
	this->_selects = new vector<SelectTransaction*>();
	this->_creates = new vector<CreateTransaction*>();
	this->_inserts = new vector<InsertTransaction*>();
	this->_updates = new vector<UpdateTransaction*>();
}

Database::Database(const string name)
{
	sqlite3_open(name.c_str(), &this->_db); // TODO: check return for SQLITE_OK
	this->_selects = new vector<SelectTransaction*>();
	this->_creates = new vector<CreateTransaction*>();
	this->_inserts = new vector<InsertTransaction*>();
	this->_updates = new vector<UpdateTransaction*>();
}

Database::~Database()
{
	sqlite3_close(this->_db); // TODO check for SQLITE_OK
	delete this->_selects;
	delete this->_creates;
	delete this->_inserts;
}

void Database::commit()
{

	WG_ITERATE_PTR(it, vector<CreateTransaction*>, this->_creates)
		execute((*it)->build());

	WG_ITERATE_PTR(it, vector<InsertTransaction*>, this->_inserts)
		execute((*it)->build());

	WG_ITERATE_PTR(it, vector<UpdateTransaction*>, this->_updates)
		execute((*it)->build());

	WG_ITERATE_PTR(it, vector<SelectTransaction*>, this->_selects)
		execute((*it)->build());
}

void Database::execute(const string sql)
{
	// WG_LOG(sql);
	int status = sqlite3_exec(this->_db, sql.c_str(), Database::_callback, NULL, &this->_errors);
	if (status != SQLITE_OK)
		WG_LOG(sqlite3_errmsg(this->_db));
}

SelectTransaction* Database::query()
{
	SelectTransaction* transaction = new SelectTransaction();
	this->_selects->push_back(transaction);
	return transaction;
}

CreateTransaction* Database::create(string name)
{
	CreateTransaction* transaction = new CreateTransaction(name);
	this->_creates->push_back(transaction);
	return transaction;
}

InsertTransaction* Database::insert(string name)
{
	InsertTransaction* transaction = new InsertTransaction(name);
	this->_inserts->push_back(transaction);
	return transaction;
}

UpdateTransaction* Database::update(string name)
{
	UpdateTransaction* transaction = new UpdateTransaction(name);
	this->_updates->push_back(transaction);
	return transaction;
}

#ifdef WG_Cpp11

SelectTransaction* Database::query(function<void(SelectTransaction*)> callback)
{
	SelectTransaction* transaction = new SelectTransaction();
	this->_selects->push_back(transaction);
	callback(transaction);
	return transaction;
}

CreateTransaction* Database::create(string name, function<void(CreateTransaction*)> callback)
{
	CreateTransaction* transaction = new CreateTransaction(name);
	this->_creates->push_back(transaction);
	callback(transaction);
	return transaction;
}

InsertTransaction* Database::insert(string name, function<void(InsertTransaction*)> callback)
{
	InsertTransaction* transaction = new InsertTransaction(name);
	this->_inserts->push_back(transaction);
	callback(transaction);
	return transaction;
}

UpdateTransaction* Database::update(string name, function<void(UpdateTransaction*)> callback)
{
	UpdateTransaction* transaction = new UpdateTransaction(name);
	this->_updates->push_back(transaction);
	callback(transaction);
	return transaction;
}

#else

// TODO: old school function pointers come here

#endif

int Database::_callback(void* resp, int rowc, char** fields, char** columns)
{
	map<string, string> &result = (map<string, string>&)resp;
	for (int i = 0; i < rowc; i++) // TODO: insert into rows
		result[columns[i]] = fields[i];
	return SQLITE_OK;
}
