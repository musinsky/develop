c     2013-06-20

      program ascii2bin
      dimension a(1000)

      open(1,file='dub57.asc',status='OLD',
     +     iostat=ios1)
      if (ios1 .gt. 0) then
         print *,'iostat = ',ios1
         stop 1
      endif
      open(2,file='dub57.bin',form='UNFORMATTED',status='REPLACE')

      nev=0
  100 read(1,*,end=200,err=300) nw,(a(i),i=1,nw)
      if (nw .gt. 1000) then
         print *,'skip event ',nev
         goto 100
      endif
      write(2,err=300) nw,(a(i),i=1,nw)
      nev=nev+1
      goto 100

  300 continue
      print *,'error problem'
  200 continue
      print *,'end of file'
      print *,'total events = ',nev
      close(1)
      close(2)
      stop
      end
