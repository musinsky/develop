// 2023-11-07

// $ gcc -o sqlite04 sqlite04.c -lsqlite3 && ./sqlite04

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

static int exec_callback(void *user, int ncols, char **col_values, char **col_names) {
  if (user) printf("%s\n", (const char *)user);
  for(int i = 0; i < ncols; i++) {
    printf("arg[%d]: %s = %s\n", i, col_names[i], col_values[i] ? col_values[i] : "NULL");
  }
  printf("\n");
  return 0;
}


int main(void) {
  sqlite3 *db;   // SQLite db handle
  char *sql;     // sql statement

  if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
    fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  FILE *fp = fopen("dst.files.sql", "r");
  if (!fp) { perror("fopen"); return 1; }
  fseek(fp, 0, SEEK_END);
  long fsize = ftell(fp);
  fseek(fp, 0, SEEK_SET); // rewind to beginning of the file
  char *fbulk = (char *)malloc(fsize);
  fread(fbulk, fsize, 1, fp);
  // // contents of the text file is properly terminated by '\0'
  // printf("file size: %ld\n", fsize);
  // for (int i = 0; i < fsize; i++) printf("%c", fbulk[i]);

  if (sqlite3_exec(db, fbulk, NULL, NULL, NULL) != SQLITE_OK) {
    fprintf(stderr, "Error executing SQL query: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }
  free(fbulk);
  fclose(fp);

  sql = "SELECT * FROM DST_files WHERE prefix='he4p8'";
  if (sqlite3_exec(db, sql, exec_callback, NULL, NULL) != SQLITE_OK) {
    fprintf(stderr, "Error executing SQL query: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  sql = "PRAGMA table_info(DST_files)";
  if (sqlite3_exec(db, sql, exec_callback, NULL, NULL) != SQLITE_OK) {
    fprintf(stderr, "Error executing SQL query: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  sqlite3_close(db);
  return 0;
}
