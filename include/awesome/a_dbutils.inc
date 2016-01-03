#if defined _dbutils_included
	#endinput
#endif
#define _dbutils_included

#include <a_sampdb>
#include <a_samp>

stock DB:DBUTILS_initDatabase(dbname[])
{
	new DB:res;
	if((res = db_open(dbname)) == DB:0)
	{
		print("Failed to open a connection to \"test.db\".");
		SendRconCommand("exit");
		return DB:-1;
	}
	print("Successfully created a connection to \"test.db\".");
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
