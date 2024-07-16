// 2024-07-16

// $ gcc -o sqlite05 sqlite05.c -lsqlite3 && ./sqlite05

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

static int exec_callback(void *user, int ncols, char **col_values, char **col_names) {
  if (user) printf("%s\n", (const char *)user);
  for(int i = 0; i < ncols; i++) {
    printf("column[%d]: %s = %s\n", i, col_names[i], col_values[i] ? col_values[i] : "NULL");
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
  char *fbulk = (char *)malloc(fsize+1);
  fbulk[fsize] = '\0'; // terminating null byte '\0';
  // memset(fbulk, 0, fsize+1); // or simalar solution
  fread(fbulk, fsize, 1, fp);
  // // contents of the text file must properly terminated by '\0' (null byte)
  // // for example find 'fread' on https://sqlite.org/src/file/src/shell.c.in
  // printf("file size: %ld\n", fsize);
  // for (int i = 0; i < fsize; i++) printf("%c", fbulk[i]);
  // printf("no last char: '%c' hex 0x%X\n", fbulk[fsize-1], fbulk[fsize-1]);
  // printf("   last char: '%c' hex 0x%X\n", fbulk[fsize], fbulk[fsize]);

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
