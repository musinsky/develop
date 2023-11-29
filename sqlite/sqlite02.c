// 2023-10-17

// $ gcc -o sqlite01a sqlite01a.c -lsqlite3 && ./sqlite01a

#include <stdio.h>
#include <sqlite3.h>

static int exec_callback(void *NotUsed, int argc, char **argv, char **azColName) {
  // If the callback function of the 3rd argument to sqlite3_exec() is not NULL, then it
  // is invoked for each result row coming out of the evaluated SQL statements.
  //
  // 1st argument is relayed through to the 4th argument to sqlite3_exec(), often not used.
  // 2nd argument is the number of columns in the result.
  // 3rd argument is an array of pointers to strings obtained as if from
  // sqlite3_column_text(), one for each column.
  // 4th argument is an array of pointers to strings where each entry represents the name
  // of corresponding result column as obtained from sqlite3_column_name().

  if (NotUsed) printf("callback 1st argument: %s\n", (const char *)NotUsed);
  for(int i = 0; i < argc; i++) {
    printf("callback arg[%d]: %s = %s\n", i, azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0; // if non-zero, the sqlite3_exec() routine returns SQLITE_ABORT
}

int main(void) {
  sqlite3 *db;   // SQLite db handle
  char *err_msg; // error msg
  int rc;        // return code

  rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  // https://sqlite.org/c3ref/exec.html
  // A wrapper function that does sqlite3_prepare_v2(), sqlite3_step(), sqlite3_column()
  // and sqlite3_finalize() for a string of one or more SQL statements
  //  rc = sqlite3_exec(db, "SELECT SQLITE_VERSION()", NULL, NULL, &err_msg);
  rc = sqlite3_exec(db, "SELECT SQLITE_VERSION()", exec_callback, NULL, &err_msg);
  // aka one step query execution interface
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Error executing SQL query: %s\n", err_msg);
    // https://sqlite.org/c3ref/free.html
    // memory allocated message string must be freed
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return 1;
  }

  sqlite3_close(db);

  return 0;
}
