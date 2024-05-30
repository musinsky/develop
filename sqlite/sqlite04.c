// 2023-10-18

// $ gcc -o sqlite04 sqlite04.c -lsqlite3 && ./sqlite04

#include <stdio.h>
#include <sqlite3.h>

// https://sqlite.org/backup.html
int loadOrSaveDb(sqlite3 *pInMemory, const char *zFilename, int isSave) {
  int rc;                   /* Function return code */
  sqlite3 *pFile;           /* Database connection opened on zFilename */
  sqlite3_backup *pBackup;  /* Backup object used to copy data */
  sqlite3 *pTo;             /* Database to copy to (pFile or pInMemory) */
  sqlite3 *pFrom;           /* Database to copy from (pFile or pInMemory) */

  rc = sqlite3_open(zFilename, &pFile);
  if (rc == SQLITE_OK) {
    pFrom = (isSave ? pInMemory : pFile);
    pTo   = (isSave ? pFile     : pInMemory);
    pBackup = sqlite3_backup_init(pTo, "main", pFrom, "main");
    if (pBackup) {
      (void)sqlite3_backup_step(pBackup, -1);
      (void)sqlite3_backup_finish(pBackup);
    }
    rc = sqlite3_errcode(pTo);
  }

  (void)sqlite3_close(pFile);
  return rc;
}

int main(void) {
  sqlite3 *db;   // SQLite db handle
  char *sql;     // sql statement

  if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
    fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  sql = "CREATE TABLE test (name TEXT, age INTEGER);"
    "INSERT INTO test VALUES('Jan', 11);"
    "INSERT INTO test VALUES('Eva', 22);"
    "INSERT INTO test VALUES('Ema', 33);";
  if (sqlite3_exec(db, sql, NULL, NULL, NULL) != SQLITE_OK) {
    //fprintf(stderr, "Error executing SQL query\n");
    fprintf(stderr, "Error executing SQL query: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  // saving SQLite db (from memory) into file
  if (loadOrSaveDb(db, "test.db", 1) != SQLITE_OK)
    fprintf(stderr, "Error loadOrSaveDb (save from memory to file)\n");
  else
    fprintf(stdout, "SQLite db 'test.db' saved\n");

  sqlite3_close(db);
  db = NULL; // not necessary

  // loading SQLite db (from file) into memory
  sqlite3_open(":memory:", &db);
  if (loadOrSaveDb(db, "test.db", 0) != SQLITE_OK)
    fprintf(stderr, "Error loadOrSaveDb (load from file to memory)\n");

  sqlite3_close(db);
  return 0;
}
