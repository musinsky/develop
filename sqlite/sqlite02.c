// 2023-11-07

// $ gcc -o sqlite02 sqlite02.c -lsqlite3 && ./sqlite02

#include <stdio.h>
#include <sqlite3.h>

static int exec_callback(void *user, int ncols, char **col_values, char **col_names) {
  // sqlite3_exec() callback is invoked for each result row coming out of the evaluated
  // SQL statements (sqlite3_exec() wrapper called i.a. sqlite3_column() function)
  printf("%s\n", (const char *)user);
  for(int i = 0; i < ncols; i++) {
    printf("arg[%d]: %s = %s\n", i, col_names[i], col_values[i] ? col_values[i] : "NULL");
  }
  printf("\n");
  return 0;
}

int main(void) {
  sqlite3 *db;   // SQLite db handle
  char *err_msg; // error msg
  char *sql;     // sql statement
  int rc;        // return code

  rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  char *user_data = "Callback function called";
  sql = "CREATE TABLE test (name TEXT, age INTEGER);"
    "INSERT INTO test VALUES('Jan', 11);"
    "INSERT INTO test VALUES('Eva', 22);"
    "INSERT INTO test VALUES('Ema', 33);";
  //rc = sqlite3_exec(db, sql, exec_callback, user_data, &err_msg); // callback not called
  rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
  sql = "INSERT INTO test VALUES('Mia', 44);";
  //rc = sqlite3_exec(db, sql, exec_callback, user_data, &err_msg); // callback not called
  rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
  sql = "SELECT * FROM test;";
  rc = sqlite3_exec(db, sql, exec_callback, user_data, &err_msg);
  user_data = "Callback function called 2nd";
  sql = "INSERT INTO test VALUES('Oto', 55);"
    "SELECT rowid, name, age FROM test;";
  rc = sqlite3_exec(db, sql, exec_callback, user_data, &err_msg);

  // ideal test after each call sqlite3_exec()
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Error executing SQL query: %s\n", err_msg);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return 1;
  }

  // https://sqlite.org/c3ref/last_insert_rowid.html
  printf("Last insert rowid: %lld\n", sqlite3_last_insert_rowid(db));

  sqlite3_close(db);
  return 0;
}
