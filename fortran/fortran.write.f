c     2023-08-17
c
c     $ gfortran -o fortran.write fortran.write.f
c
c     CERNLIB je napisany v F77 standarde, ten ma vsak znacne obmedzenia: len velke
c     pismena, nazov premennej max. 6 znakov, no empty line a pod.
c     'g77' was the predecessor to 'gfortran'. As of gcc 4.0 (2005-04), it is no longer
c     actively supported.
c     'gfortran' pouziva superset poslednych Fortran standardov (77, 90 a 95 uplne,
c     vacsinu z 2003 a 2008 standardov a niektore vlastnosti z 2018 standardu).
c
c     https://gcc.gnu.org/wiki/GFortranStandards

      PROGRAM fortran_write
c     https://www.obliquity.com/computer/fortran/io.html
c
c     OPEN(UNIT,FILE,STATUS,ERR,IOSTAT,FORM,ACCESS,RECL,BLANK)   # F77 standard
c     UNIT number must always be given
c     STATUS: default is 'UNKNOWN'    # 'OLD', 'NEW', 'UNKNOWN' and 'SCRATCH'
c     FORM:   default is 'FORMATTED'  # 'FORMATTED' or 'UNFORMATTED'
c     ACCESS: default is 'SEQUENTIAL' # 'SEQUENTIAL' or 'DIRECT'
c
c     CLOSE(UNIT,STATUS,ERR,IOSTAT)                              # F77 standard
c     UNIT number must always be given
c     STATUS: default is 'KEEP'       # 'KEEP' or 'DELETE'

      INTEGER    I1, I2
      REAL       R1, R2
      PARAMETER (NA=5)
      REAL       ARRAY(NA)
      I1=12345
      I2=-7
      R1=3.14                   ! 3.14159265359
      R2=-999.0
      DO 10, I = 1,NA
         ARRAY(I) = I*1.1
   10 CONTINUE

      OPEN(UNIT=30, FILE='fortran.write.FMT.SEQ.txt')
      WRITE(UNIT=30, FMT=100) I1, I2, R1, R2
      WRITE(UNIT=30, FMT=101) NA, (ARRAY(I), I=1,NA)
c     https://www.obliquity.com/computer/fortran/format.html
  100 FORMAT(2I10, 2F12.6)      ! 2x int, 2x floating point data
  101 FORMAT(I3, 5F10.7)        ! 1x int, 5x floating point data
      CALL inquire_unit(30)
      CLOSE(UNIT=30)

      OPEN(UNIT=31, FILE='fortran.write.UNFMT.SEQ.bin',
     &     FORM='UNFORMATTED')
c     $ gfortran -frecord-marker=8   # default length is 4 (for gcc/gfortran 4.2+)
      WRITE(UNIT=31) I1, I2, R1, R2
      WRITE(UNIT=31) NA, (ARRAY(I), I=1,NA)
c     WRITE(UNIT=31) NA, ARRAY
      CALL inquire_unit(31)
      CLOSE(UNIT=31)

      OPEN(UNIT=32, FILE='fortran.write.UNFMT.STREAM.bin',
     &     FORM='UNFORMATTED',
     &     ACCESS='STREAM', STATUS='REPLACE') ! F2003 standard
      WRITE(UNIT=32) I1, I2, R1, R2
      WRITE(UNIT=32) NA, (ARRAY(I), I=1,NA)
c     WRITE(UNIT=32) NA, ARRAY
      CALL inquire_unit(32)
      CLOSE(UNIT=32)

      STOP "End of 'fortran_write' program"
      END


      SUBROUTINE inquire_unit(unit_num)
      INTEGER      unit_num
c     F77 standard
c     https://docs.oracle.com/cd/E19957-01/802-2998/802-2998.pdf   # (page 199/173)
      INTEGER      q_iostat, q_number,    q_recl,        q_nextrec
      CHARACTER*30 q_name,   q_access,    q_direct,      q_sequential
      CHARACTER*30 q_form,   q_formatted, q_unformatted, q_blank
      LOGICAL      q_exist,  q_opened,    q_named
c     F2003 standard
c     https://j3-fortran.org/doc/year/04/04-007.pdf   # (page 226/210)
      INTEGER      q_size
      CHARACTER*30 q_action, q_stream

      INQUIRE(UNIT=unit_num,
c     F77 standard
     &     IOSTAT=q_iostat, NUMBER=q_number, RECL=q_recl,
     &     NEXTREC=q_nextrec, NAME=q_name, ACCESS=q_access,
     &     DIRECT=q_direct, SEQUENTIAL=q_sequential, FORM=q_form,
     &     FORMATTED=q_formatted, UNFORMATTED=q_unformatted,
     &     BLANK=q_blank, EXIST=q_exist, OPENED=q_opened, NAMED=q_named,
c     F2003 standard
     &     SIZE=q_size, ACTION=q_action, STREAM=q_stream)

      WRITE(*,*) 'NAME: ',        q_name
      WRITE(*,*) 'NUMBER: ',      q_number
      WRITE(*,*) 'IOSTAT: ',      q_iostat
      WRITE(*,*) 'EXIST: ',       q_exist
      WRITE(*,*) 'OPENED: ',      q_opened
      WRITE(*,*) 'NAMED: ',       q_named
      WRITE(*,*) 'FORM: ',        q_form
      WRITE(*,*) 'FORMATTED: ',   q_formatted
      WRITE(*,*) 'UNFORMATTED: ', q_unformatted
      WRITE(*,*) 'ACCESS: ',      q_access
      WRITE(*,*) 'SEQUENTIAL: ',  q_sequential
      WRITE(*,*) 'DIRECT: ',      q_direct
      WRITE(*,*) 'RECL: ',        q_recl
      WRITE(*,*) 'NEXTREC: ',     q_nextrec
      WRITE(*,*) 'BLANK: ',       q_blank
      WRITE(*,*) 'SIZE   (F2003): ', q_size
      WRITE(*,*) 'ACTION (F2003): ', q_action
      WRITE(*,*) 'STREAM (F2003): ', q_stream
      WRITE(*,*)'===================================='
      END
