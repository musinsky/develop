c     2024-02-07
c
c     $ gfortran -o seq2stream seq2stream.f   # F2003 standard

      PROGRAM seq2stream
      INTEGER nbytes, nrecords, nword, ios_in, ios_out
      REAL    array(999)
      nbytes=0
      nrecords=0

c     read input fortran (SEQUENTIAL by default) binary file
      OPEN(UNIT=10, FILE='DST_1m_HBC_CD1997/dp/all2pr.dvo',
     &     STATUS='OLD', FORM='UNFORMATTED', IOSTAT=ios_in)
      IF (ios_in .NE. 0) GOTO 22

c     write output fortran STREAM binary file (F2003 standard)
      OPEN(UNIT=20, FILE='dp.mini.2pr.fsbin',
     &     ACCESS='STREAM',
     &     STATUS='REPLACE', FORM='UNFORMATTED', IOSTAT=ios_out)
      IF (ios_out .NE. 0) GOTO 22

   99 READ(UNIT=10, END=33, ERR=22)  nword, (array(i), i=1, nword)
      WRITE(UNIT=20, ERR=22)         nword, (array(i), i=1, nword)
      nbytes=nbytes + 4 + 4*nword ! each word of 4 bytes size
      nrecords=nrecords + 1
      GOTO 99

   22 WRITE(*,*) 'Error in program'
   33 WRITE(*,*) 'End of file'
      WRITE(*,*) 'nbytes  :', nbytes
      WRITE(*,*) 'nrecords:', nrecords
      CLOSE(UNIT=20)
      CLOSE(UNIT=10)
      STOP "End of 'seq2stream' program"
      END
