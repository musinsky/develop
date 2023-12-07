// 2023-10-17

// $ gcc -o sqlite01 sqlite01.c -lsqlite3 && ./sqlite01

#include <stdio.h>
#include <sqlite3.h>

int main(void) {
  // https://sqlite.org/c3ref/libversion.html
  printf("libversion: %s\nlibversion_number: %d\n",
         sqlite3_libversion(), sqlite3_libversion_number());

  sqlite3 *db;        // SQLite db handle
  sqlite3_stmt *stmt; // Statement handle
  int rc;             // return code

  // https://sqlite.org/c3ref/open.html
  // Open a connection to a new or existing SQLite database
  rc = sqlite3_open(":memory:", &db);
  //rc = sqlite3_open("/non/exist.db", &db);
  // bez ohladu na to, ci sa vyskytne chyba alebo nie, bude potrebne volat
  // funkciu sqlite3_close() na uvolnenie prostriedkov (pamati)
  //
  // sqlite3_open_v2() works like sqlite3_open(), ale umoznuju dodatocnu
  // kontrolu over the new database connection

  if (rc != SQLITE_OK) { // https://sqlite.org/rescode.html#ok (SQLITE_OK=0)
    // https://sqlite.org/c3ref/errcode.html
    fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  // https://sqlite.org/c3ref/stmt.html
  // Single SQL statement that has been compiled into binary form and is ready to be evaluated

  // https://sqlite.org/c3ref/prepare.html
  // Compile SQL text into byte-code that will do the work of querying or updating the database
  rc = sqlite3_prepare_v2(db, "SELECT SQLITE_VERSION()", -1, &stmt, NULL);
  // aka execute sql statement/query or aka get/fetch data
  // sqlite3_prepare_v2() is preferred, sqlite3_prepare() is legacy and should be avoided
  // sqlite3_prepare_v3() is used for special purposes

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Error compiling SQL statement: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  // https://sqlite.org/c3ref/step.html
  // Advance an sqlite3_stmt to the next result row or to completion
  rc = sqlite3_step(stmt);
  // aka evaluate an SQL statement aka get record one by one
  // sqlite3_step() returns SQLITE_ROW(100) for each record in the set until
  // it reaches the end, whereupon it returns SQLITE_DONE(101)
  if (rc == SQLITE_ROW) { // https://sqlite.org/rescode.html#row (SQLITE_ROW=100)
    // https://sqlite.org/c3ref/column_blob.html
    // Column values in the current result row for an sqlite3_stmt
    printf("%s\n", sqlite3_column_text(stmt, 0));
    // aka result values from a query
  }

  // https://sqlite.org/c3ref/finalize.html
  // Destructor for sqlite3_stmt
  sqlite3_finalize(stmt);
  // aka destroy a prepared statement object
  // or sqlite3_reset() to reset a prepared statement, ready for re-executed (re-used)

  // https://sqlite.org/c3ref/close.html
  // Destructor for sqlite3
  sqlite3_close(db);
  // aka closing a database connection

  return 0;
}
