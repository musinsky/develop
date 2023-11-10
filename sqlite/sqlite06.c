// 2023-11-10

// $ gcc -o sqlite06 sqlite06.c -lsqlite3 && ./sqlite06

#include <stdio.h>
#include <sqlite3.h>

static int print_callback(void *user, int ncols, char **col_values, char **col_names) {
  static void *last = NULL;
  if (last != user) { // print header with column names
    for(int i = 0; i < ncols; i++)
      printf("#%s#\t", col_names[i]);
    printf("\n");
  }
  last = user;
  for(int i = 0; i < ncols; i++)
    printf(" %s\t", col_values[i] ? col_values[i] : "NULL");
  printf("# %s\n", (const char *)user);
  return 0;
}

void print_tail(const char *ptail)
{
  if (ptail[0] == '\0')
    printf("tail: no\n");
  else
    printf("tail: %s\n", ptail);
}

int main(void) {
  sqlite3 *db;        // SQLite db handle
  char *sql;          // sql statement
  sqlite3_stmt *stmt; // statement handle
  const char *tail;   // pointer to unused portion of sql

  if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
    fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  sql = "CREATE TABLE test (name TEXT, age INTEGER);"
    "INSERT INTO test VALUES('Jan', 11);";
  if (sqlite3_exec(db, sql, NULL, NULL, NULL) != SQLITE_OK) {
    fprintf(stderr, "Error executing SQL query: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }
  sqlite3_exec(db, "SELECT * FROM test", print_callback, "1st callback", NULL);
  printf("\n\n");

  // no tail
  sql = "INSERT INTO test VALUES('Eva', 22)";
  printf("sql : %s\n", sql);
  if (sqlite3_prepare_v2(db, sql, -1, &stmt, &tail) != SQLITE_OK) {
    fprintf(stderr, "Error compiling SQL statement: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }
  print_tail(tail);
  if (sqlite3_step(stmt) != SQLITE_DONE)
    fprintf(stderr, "statement did not finished executing successfully\n");
  sqlite3_exec(db, "SELECT * FROM test", print_callback, "2nd callback", NULL);
  sqlite3_finalize(stmt);
  printf("\n");

  // with tail(s)
  sql = "INSERT INTO test VALUES('Ema', 33);" // 1st statement
    "INSERT INTO test VALUES('Mia', 44);"     // 2nd statement
    "INSERT INTO test VALUES('Oto', 55);";    // 3rd statement
  do {
    printf("sql : %s\n", sql);
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, &tail) != SQLITE_OK) {
      fprintf(stderr, "Error compiling SQL statement: %s\n", sqlite3_errmsg(db));
      sqlite3_close(db);
      return 1;
    }
    print_tail(tail);
    if (sqlite3_step(stmt) != SQLITE_DONE)
      fprintf(stderr, "statement did not finished executing successfully\n");
    sqlite3_exec(db, "SELECT * FROM test", print_callback, "3rd callback", NULL);
    sqlite3_finalize(stmt);
    sql = (char *)tail;
  } while(tail[0] != '\0');

  sqlite3_close(db);
  return 0;
}
