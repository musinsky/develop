// 2023-09-25

#include <stdio.h>

#define FSIZE 4 // fortran size for integer and float

int main()
{
  // see in 'fortran.write.f'
  int   i1, i2, na;       // 4 bytes in Fortran and C
  float r1, r2, array[5]; // 4 bytes in Fortran and C
  if ((sizeof(int) != sizeof(float)) || (sizeof(int) != FSIZE)) {
    printf("int and float size must be %d bytes (same as fortran)\n", FSIZE);
    return 1;
  }

  int frm1, frm2; // leading, trailing fortran record markers (4 bytes)
  // https://gcc.gnu.org/onlinedocs/gfortran/Runtime-Options.html
  // https://gcc.gnu.org/onlinedocs/gfortran/File-format-of-unformatted-sequential-files.html
  //
  // Unformatted sequential files are stored as logical records using record
  // markers. Each logical record consists of one of more subrecords. Each
  // subrecord consists of a leading record marker, the data written by the user
  // program, and a trailing record marker. The record markers are four-byte
  // integers by default, and eight-byte integers if the
  // '-fmax-subrecord-length=8' option (which exists for backwards compability
  // only) is in effect.

  FILE *fp;
  if ((fp = fopen("fortran.io.UNFMT.SEQ.bin", "r")) == NULL) {
    perror("fopen error file 'fortran.io.UNFMT.SEQ.bin'");
    return 1;
  }
  printf("# fortran unformatted sequential file\n");
  if (fread(&frm1, FSIZE,   1, fp) != 1) printf("WRONG\n");
  if (fread(&i1,   FSIZE,   1, fp) != 1) printf("WRONG\n");
  if (fread(&i2,   FSIZE,   1, fp) != 1) printf("WRONG\n");
  if (fread(&r1,   FSIZE,   1, fp) != 1) printf("WRONG\n");
  if (fread(&r2,   FSIZE,   1, fp) != 1) printf("WRONG\n");
  if (fread(&frm2, FSIZE,   1, fp) != 1) printf("WRONG\n");
  printf("fortran record1 markers: %d, %d\n", frm1, frm2);
  printf("record1: %d, %d, %f, %f\n", i1, i2, r1, r2);
  if (fread(&frm1, FSIZE,   1, fp) != 1) printf("WRONG\n");
  if (fread(&na,   FSIZE,   1, fp) != 1) printf("WRONG\n");
  if (fread(array, FSIZE*5, 1, fp) != 1) printf("WRONG\n");
  if (fread(&frm2, FSIZE,   1, fp) != 1) printf("WRONG\n");
  printf("fortran record2 markers: %d, %d\n", frm1, frm2);
  printf("record2: %d, %f, %f, %f, %f, %f\n", na, array[0], array[1], array[2],
         array[3], array[4]);
  fclose(fp);

  printf("\n# fortran unformatted stream file\n");
  if ((fp = fopen("fortran.io.UNFMT.STREAM.bin", "r")) == NULL) {
    perror("fopen error file 'fortran.io.UNFMT.STREAM.bin'");
    return 1;
  }
  if (fread(&i1,   FSIZE,   1, fp) != 1) printf("WRONG\n");
  if (fread(&i2,   FSIZE,   1, fp) != 1) printf("WRONG\n");
  if (fread(&r1,   FSIZE,   1, fp) != 1) printf("WRONG\n");
  if (fread(&r2,   FSIZE,   1, fp) != 1) printf("WRONG\n");
  if (fread(&na,   FSIZE,   1, fp) != 1) printf("WRONG\n");
  if (fread(array, FSIZE*5, 1, fp) != 1) printf("WRONG\n");
  printf("stream: %d, %d, %f, %f\n", i1, i2, r1, r2);
  printf("stream: %d, %f, %f, %f, %f, %f\n", na, array[0], array[1], array[2],
         array[3], array[4]);
  fclose(fp);
}
