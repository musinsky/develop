// 2024-05-15

// $ gcc -o dst.files dst.files.c -lsqlite3 && ./dst.files

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sqlite3.h>

struct dst_file {
  int  id;              // file ID
  char cd_name[32];     // file name on CD-ROM
  char cd_prefix[16];   // prefix path of file on CD-ROM
  char new_name[32];    // new file name
  enum { MINI, FULL } format;                  // mini or full format DST
  enum { REAL_AS_REAL, REAL_AS_INT } mischeck; // mischeck integer instead of real in DST
};

sqlite3 *sqlite_read(const char *sqlfname);
sqlite3_stmt *sqlite_select(sqlite3 *db, const char *sql);
int sqlite_next_file(sqlite3_stmt *stmt, struct dst_file *file);
void sqlite_column_text_copy(sqlite3_stmt *stmt, int icol, char *tptr, int tsize);
void print_dst_file(const struct dst_file f);

int main()
{
  sqlite3 *db = sqlite_read("dst.files.sql");
  if (db == NULL) return EXIT_FAILURE;
  sqlite3_stmt *stmt = sqlite_select(db, "SELECT * FROM DST_files WHERE id < 99");
  if (stmt == NULL) return EXIT_FAILURE;

  struct dst_file file;
  while(sqlite_next_file(stmt, &file) == 1) {
    print_dst_file(file);
  }

  char path_full[256], path_prefix[128];
  strcpy(path_prefix, getenv("HOME"));
  strcat(path_prefix, "/DST_1m_HBC_CD1997/");
  printf("\npath_prefix: %s\n", path_prefix);
  // since SQLite 3.7.0 (2010-07-21) automatic reset after reach SQLITE_DONE
  stmt = sqlite_select(db, "SELECT * FROM DST_files WHERE prefix='he3p' AND format='mini'");
  if (stmt == NULL) return EXIT_FAILURE;
  while(sqlite_next_file(stmt, &file) == 1) {
    print_dst_file(file);
    strcpy(path_full, path_prefix);
    strcat(path_full, file.cd_prefix); strcat(path_full, "/");
    strcat(path_full, file.cd_name);   strcat(path_full, ".dvo");
    printf("path_full: %s\n", path_full);
  }

  sqlite3_finalize(stmt);
  sqlite3_close(db);
  return EXIT_SUCCESS;
}


sqlite3 *sqlite_read(const char *sqlfname)
{
  sqlite3 *db = NULL;
  if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
    fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return NULL;
  }

  FILE *fp = fopen(sqlfname, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error opening '%s' file: %s\n", sqlfname, strerror(errno));
    sqlite3_close(db);
    return NULL;
  }

  fseek(fp, 0, SEEK_END);
  long fsize = ftell(fp);
  fseek(fp, 0, SEEK_SET); // rewind to beginning of the file
  char *fbulk = (char *)malloc(fsize);
  fread(fbulk, fsize, 1, fp);
  if (sqlite3_exec(db, fbulk, NULL, NULL, NULL) != SQLITE_OK) { // read sql from file
    fprintf(stderr, "Error executing SQL query: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    db = NULL;
  }
  free(fbulk);
  fclose(fp);

  return db;
}

sqlite3_stmt *sqlite_select(sqlite3 *db, const char *sql)
{
  sqlite3_stmt *stmt = NULL;
  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "Error compiling SQL statement: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
  }
  return stmt;
}

void sqlite_column_text_copy(sqlite3_stmt *stmt, int icol, char *tptr, int tsize)
{
  if (stmt == NULL || tptr == NULL) return;

  const char *p = (const char *)sqlite3_column_text(stmt, icol);
  // sqlite3_column_text() should call before sqlite3_column_bytes()
  int s = sqlite3_column_bytes(stmt, icol); // or strlen()
  s = s + 1; // + terminating null byte ('\0')
  if (s > tsize) {
    fprintf(stderr, "text '%s' length problem: %d > %d\n", p, s, tsize);
    p = "";
  }
  strcpy(tptr, p);
}

int sqlite_next_file(sqlite3_stmt *stmt, struct dst_file *file)
{
  if (stmt == NULL || file == NULL) return 0;

  if (sqlite3_step(stmt) == SQLITE_DONE) return 0; // or != SQLITE_ROW
  // since SQLite 3.7.0 (2010-07-21) sqlite_reset() is not needed if you reach
  // SQLITE_DONE and want to step through again (automatic reset)
  file->id = sqlite3_column_int(stmt, 0);
  sqlite_column_text_copy(stmt, 1, file->cd_name,   sizeof(file->cd_name));
  sqlite_column_text_copy(stmt, 2, file->cd_prefix, sizeof(file->cd_prefix));
  sqlite_column_text_copy(stmt, 5, file->new_name,  sizeof(file->new_name));
  if (strcmp((const char *)sqlite3_column_text(stmt, 3), "mini") == 0) file->format = MINI;
  else file->format = FULL;
  if (sqlite3_column_int(stmt, 4) == 0) file->mischeck = REAL_AS_REAL;
  else file->mischeck = REAL_AS_INT;

  return 1;
}

void print_dst_file(const struct dst_file f)
{
  fprintf(stdout, "dst_file: %2d, %-12s, %-6s, %d(%s), %d(%s), %s\n",
          f.id, f.cd_name, f.cd_prefix, f.format, f.format == MINI ? "mini" : "full",
          f.mischeck, f.mischeck == REAL_AS_REAL ? "real_as_real" : "real_as_int ",
          f.new_name);
}
