// 2023-11-08

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
  }

  sqlite3_finalize(stmt);
  sqlite3_close(db);
  return 0;
}
