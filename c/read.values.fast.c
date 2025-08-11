// 2025-08-11

// $ gcc -pedantic -Wall -Wextra -o read.values.fast read.values.fast.c && ./read.values.fast
//       -DPRINT # optional
//       -O2     # optional, significantly speeds up the program

// Simple program fast reads values (ALICE ctp counters) from ASCII file.
// In loop only one "external" function atoll(), which takes up most of the
// program's runtime, but no reason to manually override this function.
// Note, that function atoll() is the same as function strtol(nptr, NULL, 10)
// without detect errors.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

int main()
{
  const char *fname="read.values.file.sample";
  FILE *f = fopen(fname, "r");
  if (f == NULL) {
    fprintf(stderr, "fopen('%s') failed: %s\n", fname, strerror(errno));
    return EXIT_FAILURE;
  }
  FILE *fout = NULL;
  fout = fopen("/tmp/zzz.read.values", "w"); // only for compare with original

  // https://stackoverflow.com/q/29785412
  // 1st) Read a small amount of the file and process it. 4096 bytes is
  // filesystem blocksize and most caches are currently 64KB aligned.
  // 2nd) Best possible case (for fast processing) is to use a statically
  // allocated array as pre-allocated memory for the structures.
  char buffer[4096*16];   // 4096*16 = 65536

  // on x86_64 platform: (unsigned) long long = (unsigned) long = (u)int64_t
  char word[20+1];        // max. 20 digits for (unsigned) long long value
  long long values[1085]; // 1085 values (ALICE ctp counters in 2025-07 file sample)
  long long epoch_sec; // exactly 10 digits
  char epoch_ns[9+1];  // max. 9 digits
  int was_dot = 0;

  size_t nread, i, n;
  size_t word_size = 0, count_lines = 0, count_values = 0;

  do {
    nread = fread(buffer, sizeof(buffer[0]), ARRAY_SIZE(buffer), f);
    for (i = 0; i < nread; i++) {

      if (buffer[i] == '.')  { // timestamp epoch seconds.nanoseconds
        word[word_size] = '\0';
        // epoch  999999999  (9 digits) => 2001-09-09 01:46:39 UTC
        // epoch 9999999999 (10 digits) => 2286-11-20 17:46:39 UTC
        // epoch 1753480801.035600400   => exactly 10 digits for seconds
        epoch_sec = atoll(word);
        if ((word_size != 10) || (epoch_sec == 0))
          fprintf(stderr, "epoch_sec '%s' does not contain 10 digits\n", word);
#ifdef PRINT
        printf("epoch_sec: '%s' (%zu-digit) => %lld\n", word, word_size, epoch_sec);
#endif
        was_dot = 1;
        word_size = 0;
        continue;
      }

      if (buffer[i] == ' ')  { // word after space
        word[word_size] = '\0';
        if (was_dot == 1) {    // timestamp epoch seconds.nanoseconds
          // epoch 1753480801.035600400 => there should be 9 digits for nanoseconds
          //       1753566637.0356004   => python print truncates trailing zeros
          // don't use numeric data type: 0356004 != 356004
          if (word_size >= ARRAY_SIZE(epoch_ns)) {
            fprintf(stderr, "word_size: %zu >= ARRAY_SIZE(epoch_ns): %zu\n",
                    word_size, ARRAY_SIZE(epoch_ns));
            epoch_ns[0] = '\0';
          } else {
            for (n = 0; n < word_size; n++) // aka memcpy
              epoch_ns[n] = word[n];
            epoch_ns[n] = '\0';
          }
#ifdef PRINT
          printf("epoch_ns: '%s' (%zu-digit) => %s\n", word, word_size, epoch_ns);
#endif
          was_dot = 0;
        } else {               // value
          values[count_values++] = atoll(word);
#ifdef PRINT
          printf("value[%4zu]: '%s' (%zu-digit) => %lld\n",
                 count_values-1, word, word_size, values[count_values-1]);
#endif
          if (count_values >= ARRAY_SIZE(values)) {
            fprintf(stderr, "FATAL error, count_values: %zu >= ARRAY_SIZE(values): %zu\n",
                    count_values, ARRAY_SIZE(values));
            return EXIT_FAILURE;
          }
        }
        word_size = 0;
        continue;
      }

      if (buffer[i] == '\n') { // LF (line feed), POSIX newline
        word[word_size] = '\0';
        // ARRAY_SIZE(values) was checked above
        values[count_values] = atoll(word);
#ifdef PRINT
        printf("Value[%4zu]: '%s' (%zu-digit) => %lld\n\n",
               count_values, word, word_size, values[count_values]);
#endif
        // number of values (ALICE ctp counters) is exactly predetermined
        if ((count_values+1) != ARRAY_SIZE(values))
          fprintf(stderr, "(count_values+1): %zu != ARRAY_SIZE(values): %zu\n",
                  (count_values+1), ARRAY_SIZE(values));
        else
          // write identical file (only for compare, verify)
          if (fout) {
            fprintf(fout, "%lld.%s", epoch_sec, epoch_ns);
            for (n = 0; n < ARRAY_SIZE(values); n++)
              fprintf(fout, " %lld", values[n]);
            fprintf(fout, "\n");
          }

        count_lines++;
        count_values = 0; // per line
        word_size = 0;
        continue;
      }

      word[word_size++] = buffer[i];
      if (word_size >= ARRAY_SIZE(word)) {
        fprintf(stderr, "FATAL error, word_size: %zu >= ARRAY_SIZE(word): %zu\n",
                word_size, ARRAY_SIZE(word));
        return EXIT_FAILURE;
      }
    }
  } while (nread == ARRAY_SIZE(buffer));
  fclose(fout);

  //  fread() does not distinguish between end-of-file and error, and callers
  //  must use feof(3) and ferror(3) to determine which occurred.
  if (feof(f) == 0) fprintf(stderr, "end-of-file indicator was not reached\n");
  fclose(f);

  printf("count_lines: %zu\n", count_lines);
  return EXIT_SUCCESS;
}
