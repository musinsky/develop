<p align="right">last edit: 2023-11-29</p>

* [SQLite — замечательная встраиваемая БД](https://habr.com/ru/articles/149356/)

## Command Line Shell For SQLite
* https://sqlite.org/cli.html


```
$ sqlite3 dst.files.db < dst.files.sql
$ sqlite3 dst.files.db
sqlite> .output dst.files.dump.sql
sqlite> .dump
sqlite> .mode csv
sqlite> .headers on
sqlite> .output dst.files.csv
sqlite> SELECT * FROM DST_files;
sqlite> .q

$ sqlite3
sqlite> .read dst.files.sql
sqlite> .q

$ sqlite3 -init dst.files.sql
sqlite> pragma table_info('DST_files');
sqlite> .mode table
sqlite> SELECT * FROM DST_files;
sqlite> SELECT new_name FROM DST_files WHERE format='full' AND real_as_int=1;
```

## SQLite datatypes
[Datatypes in SQLite](https://sqlite.org/datatype3.html) and type Affinity
(typova podobnost, pribuznost). Most SQL database engines uses static, rigid
typing, but SQLite uses a more general dynamic type system. [Flexible typing is
a feature](https://sqlite.org/flextypegood.html) of SQLite, not a bug.
Zjednodusene povedane, v pripade SQLite ma zadany data type skor charakter
"odporucania", a nie pravidla.

SQLite 3.37.0 (2021-11-27) provides [STRICT
tables](https://sqlite.org/stricttables.html) that do rigid type enforcement,
for developers who prefer that kind of thing. <br /> `CREATE TABLE test (name
TEXT, age INTEGER) STRICT;`


## SQLite notes
* SQLite is a **case insensitive**. Table names and column names can be typed in
uppercase, lowercase, or mixed case, and different capitalizations of the same
database object name can be used interchangeably.

* [SQL Comment Syntax](https://sqlite.org/lang_comment.html) begin with `--`
extend up to and including the next newline character (`-- comment`) or C-style
comments begin with `/*` and extend up to and including the next `*/` (`/*
comment */`).

* The SQL standard says that strings must use **`'single quotes'`** and
identifiers (such as table and column names), when quoted, must use `"double
quotes"`. For compatibility with MySQL, SQLite also allows `` `backticks` `` for
identifiers and for MS databases compatibility allows `[brackets]`. If possible,
always use the standard SQL quotes. More info:
[1](https://sqlite.org/lang_keywords.html),
[2](https://stackoverflow.com/questions/25141090/use-backticks-or-double-quotes-with-python-and-sqlite).

* Interactive use of sqlite3 requires ending each statement with a
**semicolon**.<br /> `sqlite> SELECT SQLITE_VERSION();`<br /> The C API does not
require that command statements end with a semicolon (for single SQL
statement).<br /> `sqlite3_exec(db, "SELECT SQLITE_VERSION()", NULL, NULL,
NULL)`

* SQLite 3.7.11 (2012-03-20) supports INSERT syntax to allow **multiple rows**
to be inserted via the VALUES clause.<br /> `sqlite> INSERT INTO test VALUES
('Eva', 22), ('Ema', 33);`
