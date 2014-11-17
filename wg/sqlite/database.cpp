#include "database.h"
using namespace wg::sqlite;

Database::Database(const char* name)
{
	sqlite3_open(name, &this->_db); // TODO: check return for SQLITE_OK
	this->_selects = new vector<SelectTransaction*>();
	this->_creates = new vector<CreateTransaction*>();
	this->_inserts = new vector<InsertTransaction*>();
	this->_updates = new vector<UpdateTransaction*>();
	this->_deletes = new vector<DeleteTransaction*>();
}

Database::Database(const string name)
{
	sqlite3_open(name.c_str(), &this->_db); // TODO: check return for SQLITE_OK
	this->_selects = new vector<SelectTransaction*>();
	this->_creates = new vector<CreateTransaction*>();
	this->_inserts = new vector<InsertTransaction*>();
	this->_updates = new vector<UpdateTransaction*>();
	this->_deletes = new vector<DeleteTransaction*>();
}

Database::~Database()
{
	sqlite3_close(this->_db); // TODO check for SQLITE_OK
	delete this->_selects;
	delete this->_creates;
	delete this->_inserts;
	delete this->_updates;
	delete this->_deletes;
}

void Database::commit()
{

	WG_ITERATE_PTR(it, vector<CreateTransaction*>, this->_creates)
		exec_create(*it);

	WG_ITERATE_PTR(it, vector<DeleteTransaction*>, this->_deletes)
		exec_delete(*it);

	WG_ITERATE_PTR(it, vector<InsertTransaction*>, this->_inserts)
		exec_insert(*it);

	WG_ITERATE_PTR(it, vector<UpdateTransaction*>, this->_updates)
		exec_update(*it);

	WG_ITERATE_PTR(it, vector<SelectTransaction*>, this->_selects)
		exec_query(*it);
}

void Database::execute(const string sql, wg_raw_callback handler)
{
	int status = sqlite3_exec(this->_db, sql.c_str(), handler, WG_NULL, &this->_errors);
	if (status != SQLITE_OK)
		WG_LOG(sqlite3_errmsg(this->_db));
}

void Database::execute(const string sql, wg_raw_callback handler, void* obj)
{
	int status = sqlite3_exec(this->_db, sql.c_str(), handler, obj, &this->_errors);
	if (status != SQLITE_OK)
		WG_LOG(sqlite3_errmsg(this->_db));
}


void Database::exec_query(SelectTransaction* transaction)
{
    select_callback callback = transaction->getCallback(); // GCC -fpermissive forbids getting address of temporary variable
    int status = sqlite3_exec(this->_db, transaction->build().c_str(), &Database::_callback, &callback, &this->_errors);
	if (status != SQLITE_OK)
		WG_LOG(sqlite3_errmsg(this->_db));
}

void Database::exec_create(CreateTransaction* transaction)
{
	int status = sqlite3_exec(this->_db, transaction->build().c_str(), &Database::_callback, WG_NULL, &this->_errors);
	if (status != SQLITE_OK)
	{
		WG_LOG(sqlite3_errmsg(this->_db));
	}
	else
	{
		(transaction->getCallback())(transaction->build());
	}
}

void Database::exec_insert(InsertTransaction* transaction)
{
	sqlite3_update_hook(this->_db, Database::_update_hook, transaction); // install update hook
	int status = sqlite3_exec(this->_db, transaction->build().c_str(), &Database::_callback, WG_NULL, &this->_errors);
	if (status != SQLITE_OK)
		WG_LOG(sqlite3_errmsg(this->_db));
}

void Database::exec_update(UpdateTransaction* transaction)
{
	sqlite3_update_hook(this->_db, Database::_update_hook, transaction); // install update hook
	int status = sqlite3_exec(this->_db, transaction->build().c_str(), &Database::_callback, WG_NULL, &this->_errors);
	if (status != SQLITE_OK)
	{
		WG_LOG(transaction->build().c_str());
		WG_LOG(sqlite3_errmsg(this->_db));
	}
}

void Database::exec_delete(DeleteTransaction* transaction)
{
	sqlite3_update_hook(this->_db, Database::_update_hook, transaction); // install update hook
	int status = sqlite3_exec(this->_db, transaction->build().c_str(), &Database::_callback, WG_NULL, &this->_errors);
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

DeleteTransaction* Database::remove(string name)
{
	DeleteTransaction* transaction = new DeleteTransaction(name);
	this->_deletes->push_back(transaction);
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


DeleteTransaction* Database::remove(string name, function<void(DeleteTransaction*)> callback)
{
	DeleteTransaction* transaction = new DeleteTransaction(name);
	this->_deletes->push_back(transaction);
	callback(transaction);
	return transaction;
}

#else

SelectTransaction* Database::query(void(*callback)(SelectTransaction*))
{
	SelectTransaction* transaction = new SelectTransaction();
	this->_selects->push_back(transaction);
	callback(transaction);
	return transaction;
}

CreateTransaction* Database::create(string name, void(*callback)(CreateTransaction*))
{
	CreateTransaction* transaction = new CreateTransaction(name);
	this->_creates->push_back(transaction);
	callback(transaction);
	return transaction;
}

InsertTransaction* Database::insert(string name, void(*callback)(InsertTransaction*))
{
	InsertTransaction* transaction = new InsertTransaction(name);
	this->_inserts->push_back(transaction);
	callback(transaction);
	return transaction;
}

UpdateTransaction* Database::update(string name, void(*callback)(UpdateTransaction*))
{
	UpdateTransaction* transaction = new UpdateTransaction(name);
	this->_updates->push_back(transaction);
	callback(transaction);
	return transaction;
}

DeleteTransaction* Database::removes(string name, void(*callback)(DeleteTransaction*))
{
	DeleteTransaction* transaction = new DeleteTransaction(name);
	this->_deletes->push_back(transaction);
	callback(transaction);
	return transaction;
}

#endif

int Database::_callback(void* resp, int rowc, char** fields, char** columns)
{
	select_callback &callback = (select_callback&)resp; // TODO: call callback
	//cout << "[SELECT] rowc: " << rowc << endl;
	return SQLITE_OK;
}

void Database::_update_hook(void* callback, int type, char const * db, char const * table, sqlite3_int64 rowid)
{
	switch (type)
	{
		case SQLITE_INSERT:
		{
			InsertTransaction* transaction = static_cast<InsertTransaction*>(callback);
			(transaction->getCallback())(static_cast<int>(rowid)); // avoid loss of data warning
			break;
		}
		case SQLITE_DELETE:
		{
			DeleteTransaction* transaction = static_cast<DeleteTransaction*>(callback); // I know it doesn't make sense but I forgot the delete ^^'
			(transaction->getCallback())(static_cast<int>(rowid)); // avoid loss of data warning
			break;
		}
		case SQLITE_UPDATE:
		{
			UpdateTransaction* transaction = static_cast<UpdateTransaction*>(callback);
			(transaction->getCallback())(static_cast<int>(rowid)); // avoid loss of data warning
			break;
		}
		default:
			break;
	}
}
