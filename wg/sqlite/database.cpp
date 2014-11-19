#include "database.h"
using namespace wg::sqlite;

Database::Database(const char* name)
{
	int status = sqlite3_open(name, &this->_db);
	if (status != SQLITE_OK)
		throw OpenException(sqlite3_errstr(status));
	this->initialize();
}

Database::Database(const string name)
{
	int status = sqlite3_open(name.c_str(), &this->_db);
	if (status != SQLITE_OK)
		throw OpenException(sqlite3_errstr(status));
	this->initialize();
}

void Database::initialize()
{
	this->_selects = new vector<SelectTransaction*>();
	this->_creates = new vector<CreateTransaction*>();
	this->_inserts = new vector<InsertTransaction*>();
	this->_updates = new vector<UpdateTransaction*>();
	this->_deletes = new vector<DeleteTransaction*>();
	sqlite3_update_hook(this->_db, Database::_update_hook, WG_NULL); // install update hook
}

Database::~Database()
{
	int status = sqlite3_close(this->_db);
	if (status != SQLITE_OK) // TODO: store status int and if not SQLITE_OK request sqlite3_errstr message
		throw CloseException(sqlite3_errmsg(this->_db));
	delete this->_selects;
	delete this->_creates;
	delete this->_inserts;
	delete this->_updates;
	delete this->_deletes;
}

void Database::commit()
{

	WG_ITERATE_PTR(it, vector<CreateTransaction*>, this->_creates)
		this->_exec(*it);

	WG_ITERATE_PTR(it, vector<DeleteTransaction*>, this->_deletes)
		this->_exec(*it);

	WG_ITERATE_PTR(it, vector<InsertTransaction*>, this->_inserts)
		this->_exec(*it);

	WG_ITERATE_PTR(it, vector<UpdateTransaction*>, this->_updates)
		this->_exec(*it);

	WG_ITERATE_PTR(it, vector<SelectTransaction*>, this->_selects)
		this->_exec(*it);
}

void Database::execute(const string sql, wg_raw_callback handler, void* obj)
{
	int status = sqlite3_exec(this->_db, sql.c_str(), handler, obj, &this->_errors);
	if (status != SQLITE_OK)
		throw QueryException(sqlite3_errmsg(this->_db));
}

void Database::_exec(SelectTransaction* trans)
{
	select_callback callback = trans->getCallback(); // GCC -fpermissive forbids getting address of temporary variable
	int status = sqlite3_exec(this->_db, trans->build().c_str(), &Database::_callback, &callback, &this->_errors);
	if (status != SQLITE_OK)
		//cout << "invalid SQL: " << trans->build() << endl;
		throw QueryException(sqlite3_errmsg(this->_db));
}

void Database::_exec(CreateTransaction* trans)
{
	int status = sqlite3_exec(this->_db, trans->build().c_str(), &Database::_callback, WG_NULL, &this->_errors);
	if (status != SQLITE_OK)
		throw QueryException(sqlite3_errmsg(this->_db));
}

void Database::_exec(InsertTransaction* trans)
{
	int status = sqlite3_exec(this->_db, trans->build().c_str(), &Database::_callback, WG_NULL, &this->_errors);
	if (status != SQLITE_OK)
		throw QueryException(sqlite3_errmsg(this->_db));
}

void Database::_exec(UpdateTransaction* trans)
{
	int status = sqlite3_exec(this->_db, trans->build().c_str(), &Database::_callback, WG_NULL, &this->_errors);
	if (status != SQLITE_OK)
		throw QueryException(sqlite3_errmsg(this->_db));
}

void Database::_exec(DeleteTransaction* trans)
{
	int status = sqlite3_exec(this->_db, trans->build().c_str(), &Database::_callback, WG_NULL, &this->_errors);
	if (status != SQLITE_OK)
		throw QueryException(sqlite3_errmsg(this->_db));
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

SelectTransaction* Database::query(std::function<void(SelectTransaction*)> callback)
{
	SelectTransaction* transaction = new SelectTransaction();
	this->_selects->push_back(transaction);
	callback(transaction);
	return transaction;
}

CreateTransaction* Database::create(string name, std::function<void(CreateTransaction*)> callback)
{
	CreateTransaction* transaction = new CreateTransaction(name);
	this->_creates->push_back(transaction);
	callback(transaction);
	return transaction;
}

InsertTransaction* Database::insert(string name, std::function<void(InsertTransaction*)> callback)
{
	InsertTransaction* transaction = new InsertTransaction(name);
	this->_inserts->push_back(transaction);
	callback(transaction);
	return transaction;
}

UpdateTransaction* Database::update(string name, std::function<void(UpdateTransaction*)> callback)
{
	UpdateTransaction* transaction = new UpdateTransaction(name);
	this->_updates->push_back(transaction);
	callback(transaction);
	return transaction;
}


DeleteTransaction* Database::remove(string name, std::function<void(DeleteTransaction*)> callback)
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
			(InsertTransaction::getCallback())(static_cast<int>(rowid)); // avoid loss of data warning
			break;
		case SQLITE_DELETE:
			(DeleteTransaction::getCallback())(static_cast<int>(rowid)); // avoid loss of data warning
			break;
		case SQLITE_UPDATE:
			(UpdateTransaction::getCallback())(static_cast<int>(rowid)); // avoid loss of data warning
			break;
		default:
			throw UnknownOperationException("unknown operation"); // todo: add type to exception message
	}
}
