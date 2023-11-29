// 2023-11-29

// $ gcc -o sqlite08 sqlite08.c -lsqlite3 && ./sqlite08

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sqlite3.h>

#define BUFSIZE 256
#define TABNAME "covid19"

int get_file_line(FILE *file, char *line) {
  line[0] = '\0'; // reset (must be '\0', not "\0")
  char fline[BUFSIZE] = "\0";
  if (fgets(fline, BUFSIZE, file) == NULL) return 0;
  // Date;Region;District;District_code;Gender;AgeGroup;PCR_Pos;PCR_Neg;PCR_Total
  char *c1 = strtok(fline, ";"); char *c2 = strtok(NULL, ";");
  char *c3 = strtok(NULL,  ";"); char *c4 = strtok(NULL, ";");
  char *c5 = strtok(NULL,  ";"); char *c6 = strtok(NULL, ";");
  char *c7 = strtok(NULL,  ";"); char *c8 = strtok(NULL, ";");
  char *c9 = strtok(NULL,  ";\n");
  if (strtok(NULL, ";") != NULL) printf("strtok line problem\n");
  sprintf(line, "INSERT INTO %s VALUES ('%s','%s','%s','%s','%s','%s',%d,%d,%d)",
          TABNAME, c1, c2, c3, c4, c5, c6, atoi(c7), atoi(c8), atoi(c9));
  // book "Using SQLite", 2010, Kreibich; chapter "Security and Performance"
  // sprintf is not the best choice, prefer sqlite3_bind_xxx() functions
  // https://sqlite.org/c3ref/bind_blob.html
  return 1;
}

void time_info(struct timespec t1, struct timespec t2, const char *info) {
  printf("%s: %.2f s\n", info,
         (t2.tv_nsec - t1.tv_nsec) / 1000000000.0 +
         (t2.tv_sec  - t1.tv_sec));
}

// https://stackoverflow.com/q/1711631   # Improve INSERT performance of SQLite

int main(void) {
  FILE *fp = fopen("covid19.data.csv", "r");
  //FILE *fp = fopen("covid19.data.sample.csv", "r");
  if (!fp) { printf("run shell script 'covid19.data.sh' first\n"); return 1; }
  char sql[BUFSIZE] = "\0";
  struct timespec time_start, time_stop;

  clock_gettime(CLOCK_MONOTONIC_RAW, &time_start);
  while (get_file_line(fp, sql)) {
    //printf("sql: %s\n", sql);
  }
  clock_gettime(CLOCK_MONOTONIC_RAW, &time_stop);
  time_info(time_start, time_stop, "parse file (line by line)");

  sqlite3 *db;
  sprintf(sql, "/tmp/%s.db", TABNAME); printf("database filename: %s\n", sql);
  if (sqlite3_open(sql, &db) != SQLITE_OK) {
    // if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
    fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }
  sprintf(sql, "CREATE TABLE %s (c1 TEXT, c2 TEXT, c3 TEXT, c4 TEXT, c5 TEXT, c6 TEXT,"
          " c7 INTEGER, c8 INTEGER, c9 INTEGER)", TABNAME);
  //printf("sql: %s\n", sql);
  if (sqlite3_exec(db, sql, NULL, NULL, NULL) != SQLITE_OK) {
    fprintf(stderr, "Error executing SQL query: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  rewind(fp);
  clock_gettime(CLOCK_MONOTONIC_RAW, &time_start);
  // extremely slow without these two (BEGIN/END TRANSACTION) statements
  sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL); // very important
  while (get_file_line(fp, sql)) {
    //printf("sql: %s\n", sql);
    if (sqlite3_exec(db, sql, NULL, NULL, NULL) != SQLITE_OK) {
      fprintf(stderr, "Error executing SQL query: %s\n", sqlite3_errmsg(db));
      sqlite3_close(db);
      return 1;
    }
  }
  sqlite3_exec(db, "END TRANSACTION", NULL, NULL, NULL);   // very important
  // END TRANSACTION is an alias for COMMIT; https://sqlite.org/lang_transaction.html
  clock_gettime(CLOCK_MONOTONIC_RAW, &time_stop);
  time_info(time_start, time_stop, "sql exec loop");

  sqlite3_close(db);
  fclose(fp);
  return 0;
}
