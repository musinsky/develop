c     1997-06-04
c     prc_dvo.f na VVG pocitaci, neskor tiez pod nazvom ascii2bin.f
c     povodny program na prevod allppn.dst -> allppn.dvo (ascii -> bin)

      program prc_dvo

c------perekodirovka dst->dvo------

      character*12 DST_name, DVO_name
      dimension a(1000)

      print *
      print *,' Perekodirovka DST -> DVO'
      print *,' ------------------------'
      print *

      write(*,'(" Enter name of  input DST file : ",$)')
      read *, DST_name
      open ( 11, file=DST_name, status='old', iostat=ios1 )
      if ( ios1 .gt. 0 ) then
         print *,'iostat = ',ios1
         stop 11
      endif

      write(*,'(" Enter name of output DVO file : ",$)')
      read *, DVO_name
      open ( 21 , file=DVO_name, form='unformatted',
     +     iostat=ios2, status='new')
      if ( DST_name .eq. DVO_name ) goto 200

      nev=0
    1 read ( 11, *,end=100,err=200 ) nw, (a(i), i=1,nw)
      write (21, err=200 ) nw,(a(i),i=1,nw)
      nev = nev + 1
      go to 1

  100 continue
      print *
      write (*,fmt=10) nev
   10 format(3x,'Total number of event processing        nev=',i10)
      stop

  200 print *
      print *,' Error on input or output file ...  '
      print *
      stop 33
      end
