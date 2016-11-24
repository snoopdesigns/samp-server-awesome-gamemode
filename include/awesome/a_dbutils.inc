#if defined _dbutils_included
	#endinput
#endif
#define _dbutils_included

#include <a_sampdb>
#include <a_samp>
#include "include/awesome/a_log.inc"

#define DB_FILENAME "awesome-freeroam.db"

stock DB:DBUTILS_initDatabase()
{
	LOG_write("Database Subsystem initializing");
	new DB:res;
	if((res = db_open(DB_FILENAME)) == DB:0)
	{
		LOG_write("Failed to open a connection to Awesome DB");
		SendRconCommand("exit");
		return DB:-1;
	}
	LOG_write("Successfully created a connection to Awesome DB");
	return res;
}

stock DBUTILS_shutdown(DB:db_handle_to_close)
{
	db_close(db_handle_to_close);
}

stock DBResult:DBUTILS_runSelectQuery(DB:dbhandle, query[])
{
	DBUTILS_logQuery(query);
	return db_query(dbhandle, query);
}

stock DBUTILS_runSelectQueryClear(DBResult: db_result) 
{
	db_free_result(db_result);
}

stock DBUTILS_runCreateQuery(DB:dbhandle, query[])
{
	DBUTILS_logQuery(query);
	db_free_result(db_query(dbhandle, query));
}

stock DBUTILS_runDropQuery(DB:dbhandle, table_name[])
{
	new query[256];
	format(query, sizeof(query), "DROP TABLE IF EXISTS '%s'", table_name);
	DBUTILS_logQuery(query);
	db_free_result(db_query(dbhandle, query));
}

stock DBUTILS_runInsertQuery(DB:dbhandle, query[])
{
	DBUTILS_runCreateQuery(dbhandle, query);
}

stock DBUTILS_runUpdateQuery(DB:dbhandle, query[])
{
	DBUTILS_runCreateQuery(dbhandle, query);
}

stock DBUTILS_getResultsetSize(DBResult:db_result)
{
	return db_num_rows(db_result);
}

stock DBUTILS_nextItem(DBResult:db_result)
{
	return db_next_row(db_result);
}

stock DBUTILS_getFieldFromResult(DBResult:db_result, field_name[]) 
{
	new field[128];
	db_get_field_assoc(db_result, field_name, field, sizeof field);
	return field;
}

DBUTILS_logQuery(query[]) 
{
	new string[256];
	format(string,sizeof(string),"[DB_QUERY]: %s",query);
	print(string);

}
