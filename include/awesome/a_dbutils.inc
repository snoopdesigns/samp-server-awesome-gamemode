#if defined _dbutils_included
	#endinput
#endif
#define _dbutils_included

#include <a_sampdb>
#include <a_samp>

stock DBUTILS_initDatabase(dbname[])
{
	new DB:res;
	if((res = db_open(dbname)) == DB:0)
	{
		print("Failed to open a connection to \"test.db\".");
		SendRconCommand("exit");
		return -1;
	}
	print("Successfully created a connection to \"test.db\".");
	return res;
}

stock DBUTILS_shutdown(db_handle)
{
	db_close(db_handle);
}

stock DBUTILS_runSelectQuery(dbhandle, query[])
{
	DBUTILS_logQuery(query);
	return db_query(dbhandle, query);
}

stock DBUTILS_runSelectQueryClear(db_result) 
{
	db_free_result(db_result);
}

stock DBUTILS_runCreateQuery(dbhandle, query[])
{
	DBUTILS_logQuery(query);
	db_free_result(db_query(dbhandle, query));
}

stock DBUTILS_runDropQuery(dbhandle, table_name[])
{
	new query[256];
	format(query, sizeof(query), "DROP TABLE IF EXISTS '%s'", table_name);
	DBUTILS_logQuery(query);
	db_free_result(db_query(dbhandle, query));
}

stock DBUTILS_runInsertQuery(dbhandle, query[])
{
	DBUTILS_runCreateQuery(dbhandle, query);
}

stock DBUTILS_runUpdateQuery(dbhandle, query[])
{
	DBUTILS_runCreateQuery(dbhandle, query);
}

stock DBUTILS_getResultsetSize(db_result)
{
	return db_num_rows(db_result);
}

stock DBUTILS_nextItem(db_result)
{
	return db_next_row(db_result);
}

stock DBUTILS_getFieldValueFromResultSet(db_result, field_name[]) 
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
