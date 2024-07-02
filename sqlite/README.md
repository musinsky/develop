<p align="right">last edit: 2024-07-03</p>

# SQLite
* https://en.wikipedia.org/wiki/SQLite
* https://sqlite.org

---

* https://www.sqlitetutorial.net/
* https://www.tutorialspoint.com/sqlite ([русский перевод](https://coderlessons.com/tutorials/bazy-dannykh/vyuchit-sqlite/uchebnik-po-sqlite))
* https://www.tutlane.com/tutorial/sqlite
* https://www.techonthenet.com/sqlite

## Command Line Shell For SQLite
* https://sqlite.org/cli.html
```
$ sqlite3 dst.files.db < dst.files.sql
$ sqlite3 dst.files.db
sqlite> .output dst.files.dump.sql
sqlite> .dump
sqlite> .output dst.files.csv
sqlite> .mode csv
sqlite> .headers on
sqlite> SELECT * FROM DST_files;
sqlite> .q

$ sqlite3 dst.files.db .dump > dst.files.dump.sql
$ sqlite3 dst.files.db 'SELECT * FROM DST_files'

$ sqlite3
sqlite> .read dst.files.sql
sqlite> .q
$ sqlite3 -init dst.files.sql
sqlite> PRAGMA table_info(DST_files);
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

Note that numeric arguments in parentheses that following the type name (ex:
`VARCHAR(255)`) are ignored by SQLite, SQLite does not impose any length
restrictions. Type `VARCHAR` contains the string "CHAR" and is thus assigned
`TEXT` affinity.

SQLite does not have a storage class set aside for storing dates and/or times.
Instead, the built-in [Date And Time
Functions](https://sqlite.org/lang_datefunc.html) of SQLite are capable of
storing dates and times as `TEXT`, `REAL` or `INTEGER` values.

## SQLite notes
* SQLite is a **case insensitive**. Table names and column names can be typed in
uppercase, lowercase, or mixed case, and different capitalizations of the same
database object name can be used interchangeably.

* [SQLite Comment Syntax](https://sqlite.org/lang_comment.html) begin with `--`
extend up to and including the next newline character (`-- comment`) or C-style
comments begin with `/*` and extend up to and including the next `*/` (`/*
comment */`).

* The SQL standard says that strings must use **`'single quotes'`** and
identifiers (such as table and column names), when quoted, must use `"double
quotes"`. For compatibility with MySQL, SQLite also allows `` `backticks` `` for
identifiers and for MS databases compatibility allows `[brackets]`. If possible,
always use the standard SQL quotes. More info:
[[1]](https://sqlite.org/lang_keywords.html),
[[2]](https://stackoverflow.com/questions/25141090/use-backticks-or-double-quotes-with-python-and-sqlite).

* [SQLite operators](https://www.sqlite.org/lang_expr.html) `=` and `==` are the
same, which means equals (or `<>` and `!=` are the same, which means not
equals). It would be better to use `=` instead of `==` (or `<>` instead of `!=`)
because it's SQL standard comparison operator.

* Interactive use of sqlite3 requires ending each statement with a
**semicolon**.<br /> `sqlite> SELECT SQLITE_VERSION();`<br /> The C API does not
require that command statements end with a semicolon (for single SQL
statement).<br /> `sqlite3_exec(db, "SELECT SQLITE_VERSION()", NULL, NULL,
NULL)`

* SQLite 3.7.11 (2012-03-20) supports `INSERT` syntax to allow **multiple rows**
to be inserted via the `VALUES` clause.<br /> `sqlite> INSERT INTO test VALUES
('Eva', 22), ('Ema', 33);`

* All rows within SQLite tables have a 64-bit signed integer key that uniquely
identifies the row within its table, **rowid**. In most cases, `INTEGER PRIMARY
KEY` is alias for the **rowid**. According to the SQL standard, `PRIMARY KEY`
should always imply `NOT NULL` (and `UNIQUE`). More info:
[[1]](https://sqlite.org/lang_createtable.html).

* Be careful with `UNIQUE` constraints (obmedzenia). `NULL` values are
considered distinct (sa povazuju za odlisne) from all other values, including
other `NULL`s. More info:
[[1]](https://sqlite.org/lang_createtable.html#unique_constraints),
[[2]](https://sqlite.org/faq.html#q26),
[[3]](https://www.sqlite.org/nulls.html). Prefer way `name TEXT NOT NULL
UNIQUE`.

* [PRAGMA Statements](https://sqlite.org/pragma.html) aka SQLite metadata.
<br/>`$ echo "PRAGMA compile_options;" | sqlite3`

* `JOIN` (in SQLite) = `INNER JOIN`<br/>
`LEFT JOIN` (in SQLite) = `LEFT OUTER JOIN`

* By **default**, SQLite operates in **autocommit mode**, all changes to the
database are committed as soon as all operations associated with the current
database connection complete. Autocommit mode is disabled by a `BEGIN` statement
and re-enabled by a `COMMIT` or `ROLLBACK`. More info:
[[1]](https://sqlite.org/lang_transaction.html).

* [SQLite — замечательная встраиваемая БД](https://habr.com/ru/articles/149356/)
* [Моя большая практическая шпаргалка SQL (SQLite) с готовыми запросами](https://habr.com/ru/articles/792630/)

## C Programming Interface (C API)
* https://sqlite.org/c3ref/intro.html
* https://sqlite.org/cintro.html
* https://sqlite.org/capi3ref.html (single large HTML file)

---

* https://github.com/sqlite/sqlite/blob/master/src/shell.c.in

---

* [book: Using.SQLite.2010.Kreibich](https://www.oreilly.com/library/view/using-sqlite/9781449394592/)
* [book: The.Definitive.Guide.to.SQLite.2nd.Edition.2010.Allen](https://link.springer.com/book/10.1007/978-1-4302-3226-1)
* https://zetcode.com/db/sqlitec/
* https://souptonuts.sourceforge.net/readme_sqlite_tutorial.html
* https://stackoverflow.com/questions/tagged/sqlite+c
* [[1]](https://renenyffenegger.ch/notes/development/databases/SQLite/c-interface/basic/index),
[[2]](https://icculus.org/~chunky/stuff/sqlite3_example/)

## Others
* https://github.com/thombashi/sqlitebiter
* https://github.com/simonw/sqlite-utils
