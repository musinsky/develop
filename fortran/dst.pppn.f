c     from VVG PC: HE3/HE3P/razval/pppn.f (2010-12-13)
c     $ gfortran pppn.f $(cernlib packlib)
      program tpp
      PARAMETER (NWPAWC = 500000)
      COMMON /PAWC/ HMEMOR(NWPAWC)
      dimension a(70),pala(4,6),pa(6),ca(4),index(3),pom(4),pla(4,6),
     +     pl(6),cl(4),r(4),xpl(4),ypl(4),b(4,6),c(4,6),xm(3),pt(6),
     +     cosxz(6),tetxz(6),tkin(6),az(6),zz(6)
      data pi/3.141593/
      CALL HLIMIT(NWPAWC)
      open (11,file='he3prong.dvo',status='old',
     +     form='unformatted')
      open (6,file='pppn.dat')
c-----------------------------------------------------------------
      call hbook1 (5,'number of hyp$',20.,100.,120.,0.)
      call hbook1 (10,' mom of neutrons in alab$',50.,0.,5.5,0.)
      call hbook1 (11,'mom of protons in alab$',50.,0.,5.5,0.)
      call hbook1 (14,'t s wes$',4.,0.,0.1,0.)
      call hbook1 (15,'t$',20.,0.,0.5,0.)
c---------------------------------------------------------------------
      n=0
   11 read (11,end=100) nw, (a(i), i=1,nw)
      n=n+1
c      iflag=a(1)
c      if(a(4).lt.-1.) go to 11
c      nexp=a(1)/1000.
c      nexp=a(1)-nexp*1000.
      if(a(4).lt.112.5) go to 11
      if(a(4).gt.113.5) go to 11
c------------------------------beam--------------------------
      amp=0.93827231
      amn=0.93956563
      amd=1.87563
      amhe=3.72741
      amtri=2.80894
      pala(1,5)=a(7)
      call hfill(5,a(4),0.,1.)
      call hfill(13,a(7),0.,1.)
      pla(1,5)=a(6)
      pala(2,5)=a(9)
      pla(2,5)=a(9)
      pala(3,5)=a(10)
      pla(3,5)=a(10)
      pala(4,5)=sqrt(a(7)**2+amp**2)
      pla(4,5)=sqrt(a(6)**2+a(11)**2)
c------------------------------target-------------------------
      pla(1,6)=0.
      pla(2,6)=0.
      pla(3,6)=0.
      pla(4,6)=amp
c      do 7 l=1,4
c      b(l,4)=pala(l,4)/amp
c    7 c(l,4)=pla(l,4)/a(10)
c--------------------------end of beam-------------------------
      do 2 j=1,4
         ind=j*6+6
         pala(1,j)=a(ind+1)
         pla(1,j)=a(ind)
         pala(2,j)=a(ind+3)
         pla(2,j)=a(ind+3)
c      print *,pla(2,j)
         pala(3,j)=a(ind+4)
         pla(3,j)=a(ind+4)
         pa(j)=sqrt(pala(1,j)**2+pala(2,j)**2+pala(3,j)**2)
         pl(j)=sqrt(pla(1,j)**2+pla(2,j)**2+pla(3,j)**2)
         pala(4,j)=sqrt(pa(j)**2+a(ind+5)**2)
         pla(4,j)=sqrt(pl(j)**2+a(ind+5)**2)
         tkin(j)=pla(4,j)-a(ind+5)
         pt(j)=sqrt(pla(2,j)**2+pla(3,j)**2)
         zz(j)=pla(2,j)/pt(j)
c      print *,zz(1)
         if(zz(j).gt.1.)zz(j)=1.
         if(zz(j).lt.-1.) zz(j)=-1.
         az(j)=acos(zz(j))*180./pi
    2 continue
      xmekv=0.00254
      shag=0.025
      wes=xmekv/shag
c---------------------sort in ala system------------------------------
      call sortzv (pa,index,4,1,1,0)
      indf=index(1)
      indm=index(2)
      inds=index(3)
      indss=index(4)
c-----------------------------------d - spectator--------------
      if(indf.ne.4) go to 11
c-------------------- calcul and select t ----------------------------
      if(pa(indf).lt.3.0) go to 11
      call vsub (pla(1,6),pla(1,4),pom,4)
      t=abs(doti(pom,pom))
      call hfill(14,t,0.,wes)
      call hfill(15,t,0.,1.)
      call hfill(11,pa(1),0.,1.)
      call hfill(11,pa(2),0.,1.)
      call hfill(11,pa(3),0.,1.)
      call hfill(10,pa(4),0.,1.)
      go to 11
  100 continue
c   8  format(i9,4(2x,f10.2),/6(2x,f10.5))
      call hidopt (0,'blac')
      call histdo
      call hrput (0,'pppn.his','N')
      stop
      end
