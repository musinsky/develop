// 2023-11-09

// $ gcc -o sqlite05 sqlite05.c -lsqlite3 && ./sqlite05

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

int main(void) {
  FILE *fp = fopen("dst.files.sql", "r");
  if (!fp) { perror("fopen"); return 1; }
  fseek(fp, 0, SEEK_END);
  long fsize = ftell(fp);
  fseek(fp, 0, SEEK_SET); // rewind to beginning of the file
  char *fbulk = (char *)malloc(fsize);
  fread(fbulk, fsize, 1, fp);
  fclose(fp);

  sqlite3 *db;        // SQLite db handle
  sqlite3_stmt *stmt; // statement handle
  if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
    fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }
  if (sqlite3_exec(db, fbulk, NULL, NULL, NULL) != SQLITE_OK) { // read sql from file
    fprintf(stderr, "Error executing SQL query: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }
  free(fbulk);

  if (sqlite3_prepare_v2(db, "SELECT id, name, prefix, new_name FROM DST_files",
                         -1, &stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "Error compiling SQL statement: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
    // sqlite3_prepare_v2(db, "first statement; second statement;", -1,  &stmt, &tail)
    // WARNING: sqlite3_prepare() routines only compile the first statement in
    // 'zSql' (2nd arg), so '*pzTail' (5th arg) is left pointing to what remains
    // uncompiled. For multiple statements better choice is sqlite3_exec().
  }

  // https://sqlite.org/c3ref/column_count.html
  // https://sqlite.org/c3ref/column_name.html
  // https://sqlite.org/c3ref/column_decltype.html
  for(int ic = 0; ic < sqlite3_column_count(stmt); ic++)
    fprintf(stdout, "column[%d]: name = %s, decltype = %s\n", ic,
            sqlite3_column_name(stmt, ic), sqlite3_column_decltype(stmt, ic));
  fprintf(stdout, "\n");

  while(sqlite3_step(stmt) == SQLITE_ROW) { // or != SQLITE_DONE
    // https://sqlite.org/c3ref/column_blob.html
    fprintf(stdout, "row: id = %d, name = %s, prefix = %s, new_name = %s\n",
            sqlite3_column_int(stmt,  0), sqlite3_column_text(stmt, 1),
            sqlite3_column_text(stmt, 2), sqlite3_column_text(stmt, 3));
    // fprintf(stdout, "row type: %d, %d, %d, %d\n",
    //         // https://sqlite.org/c3ref/c_blob.html
    //         sqlite3_column_type(stmt, 0), sqlite3_column_type(stmt, 1),
    //         sqlite3_column_type(stmt, 2), sqlite3_column_type(stmt, 3));

    // TEXT or BLOB pointers returned are valid until a type conversion occurs,
    // or until sqlite3_step() or sqlite3_reset() or sqlite3_finalize() is
    // called. You'll have to copy them into memory that you control.

    // You should call sqlite3_column_text() (or BLOB) first to force the result
    // into the desired format, then invoke sqlite3_column_bytes() to find the
    // size of the result.

    // sqlite3_reset() be called before retrying sqlite3_step() after any error
    // or after SQLITE_DONE. But beginning with SQLite 3.7.0 (2010-07-21)
    // sqlite3_reset() would be called automatically (automatic reset).
  }

  sqlite3_finalize(stmt);
  sqlite3_close(db);
  return 0;
}
