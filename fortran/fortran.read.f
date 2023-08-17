c     2023-08-17
c
c     $ gfortran -o fortran.read fortran.read.f
c
c     run 'fortran.write.f' first

      PROGRAM fortran_read
      REAL ARRAY(5)

  100 FORMAT(2I10, 2F12.6)      ! 2x int, 2x floating point data
  101 FORMAT(I3, 5F10.7)        ! 1x int, 5x floating point data

      OPEN(UNIT=300, FILE='fortran.io.FMT.SEQ.txt',
     &     STATUS='OLD')
      READ(UNIT=300, FMT=100) I301, I302, R301, R302
      READ(UNIT=300, FMT=101) NA300, (ARRAY(I), I=1,NA300)
      CLOSE(UNIT=300)
      WRITE(*, *) "===== file 'fortran.io.FMT.SEQ.txt'"
      WRITE(*, FMT=100) I301, I302, R301, R302
      WRITE(*, FMT=101) NA300, (ARRAY(I), I=1,NA300)
c     WRITE(*, FMT=101) NA300, ARRAY

      OPEN(UNIT=310, FILE='fortran.io.UNFMT.SEQ.bin',
     &     STATUS='OLD', FORM='UNFORMATTED')
c     READ(UNIT=310) I311, I312, R311, R312, NA310, ARRAY   ! WRONG
      READ(UNIT=310) I311, I312, R311, R312
      READ(UNIT=310) NA310, (ARRAY(I), I=1,NA310)
      CLOSE(UNIT=310)
      WRITE(*, *) "===== file 'fortran.io.UNFMT.SEQ.bin'"
      WRITE(*, FMT=100) I311, I312, R311, R312
      WRITE(*, FMT=101) NA310, (ARRAY(I), I=1,NA310)

      OPEN(UNIT=320, FILE='fortran.io.UNFMT.STREAM.bin',
     &     STATUS='OLD', FORM='UNFORMATTED',
     &     ACCESS='STREAM')     ! F2003 standard
c     READ(UNIT=320) I321, I322, R321, R322, NA320, ARRAY   ! OK
      READ(UNIT=320) I321, I322, R321, R322
      READ(UNIT=320) NA320, (ARRAY(I), I=1,NA320)
      CLOSE(UNIT=320)
      WRITE(*, *) "===== file 'fortran.io.UNFMT.STREAM.bin'"
      WRITE(*, FMT=100) I321, I322, R321, R322
      WRITE(*, FMT=101) NA320, (ARRAY(I), I=1,NA320)

      STOP "End of 'fortran_read' program"
      END
