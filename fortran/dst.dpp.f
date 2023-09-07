c     from VVG PC: HE3/HE3P/dpp.f (2009-01-22)
c     $ gfortran dpp.f $(cernlib packlib)
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
      open (6,file='dpp.dat')
c-----------------------------------------------------------------
      call hbook1 (5,'number of hyp$',50.,0.,50.,0.)
      call hbook1 (6,'mom of deuteron$',50.,0.,1.5,0.)
      call hbook1 (7,'mom of  protons$',50.,0.,5.5,0.)
      call hbook2 (16,'mom d vs mom pr$',50.,0.,0.5,50.,0.,5.5,0.)
      call hbook1 (10,' mom of protons$',50.,1.,5.5,0.)
      call hbook1 (11,'tetcw all$',2.,0.,180.,0.)
      call hbook1 (111,'tetcw mom spect 0.-0.1$',2.,0.,180.,0.)
      call hbook1 (211,'tetcw mom spect 0.1 - 0.2$',2.,0.,180.,0.)
      call hbook1 (311,'tetcw mom spect 0.2 - 0.3$',2.,0.,180.,0.)
      call hbook1 (411,'tetcw mom spect gt 0.3$',2.,0.,180.,0.)
      call hbook1 (12,'recx$',50.,-3.,3.,0.)
      call hbook1 (13,'xbeam$',50.,-5.5,0.,0.)
      call hbook1 (14,'t$',50.,0.,1.,0.)
      call hbook2 (15,'tetcw_vs_psp$',50.,0.,180.,50.,0.,1.,0.)
      call hbook1 (17,'mom p in lab$',50.,0.,5.5,0.)
      call hbook1 (20,'azimut protona indm$',20.,0.,180.,0.)
      call hbook1 (21,'azimut protona indf$',20.,0.,180.,0.)
      call hbook1 (22,'azimut of deuterons$',20.,0.,180.,0.)
      call hbook1 (31,'transmom_Pf$',50.,0.,0.5,0.)
      call hbook1 (32,'transmom_Pm$',50.,0.,0.5,0.)
      call hbook1 (33,'transmom_Ps$',50.,0.,0.5,0.)
      call hbook1 (34,'transmom_P1$',50.,0.,0.5,0.)
      call hbook1 (35,'transmom_P2$',50.,0.,0.5,0.)
      call hbook2 (41,'Az prot vs cw$',2.,0.,180.,20.,0.,180.,0.)
c---------------------------------------------------------------------
      n=0
   11 read (11,end=100) nw, (a(i), i=1,nw)
      n=n+1
c      iflag=a(1)
c      if(a(4).lt.-1.) go to 11
c      nexp=a(1)/1000.
c      nexp=a(1)-nexp*1000.
      if(a(4).lt.0.5) go to 11
      if(a(4).gt.3.5) go to 11
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
      do 2 j=1,3
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
c---------------------sort in ala system------------------------------
      call sortzv (pa,index,3,1,1,0)
      indf=index(1)
      indm=index(2)
      inds=index(3)
c-----------------------------------d - spectator--------------
      if(inds.ne.1.) go to 11
c-------------------- calcul and select t ----------------------------
      call vsub (pla(1,6),pla(1,indf),pom,4)
      t=abs(doti(pom,pom))
      call hfill(14,t,0.,1.)
      if(t.gt.0.18) go to 11
      recx=(pala(1,5)-pala(1,indf))
      call hfill(12,recx,0.,1.)
      call hfill(15,tetcw,pa(inds),1.)
      call hfill(17,pl(2),0.,1.)
      call hfill(17,pl(3),0.,1.)
c      if(pl(2).le.0.2) go to 44
      call hfill(20,az(indm),0.,1.)
      call hfill(21,az(indf),0.,1.)
c      if(pl(3).ge.0.5) go to 44
c      call hfill(21,az(3),0.,1.)
      call hfill(22,az(1),0.,1.)
   44 continue
      recy=(pala(2,5)-pala(2,indf))
      recz=(pala(3,5)-pala(3,indf))
      recm=sqrt(recx**2+recy**2+recz**2)
c      call hfill(12,recm,0.,1.)
      palcw=pa(1)
      xx=(recx*pala(1,1)+recy*pala(2,1)+recz*pala(3,1))
     1     /(recm*palcw)
      if(xx.gt.1.) xx=1.
      if(xx.lt.-1.)xx=-1.
      tetcw=acos(xx)
      tetcw=tetcw*180./pi
      call hfill(11,tetcw,0.,1.)
      call hfill(41,tetcw,az(indm),1.)
      if(pa(inds).ge.0.1) go to 333
      if(pa(inds).le.0.) go to 333
      call hfill(111,tetcw,0.,1.)
  333 continue
      if(pa(inds).ge.0.2) go to 334
      if(pa(inds).le.0.1) go to 334
      if(az(indm).gt.60.) go to 334
      call hfill(211,tetcw,0.,1.)
c       call hfill(41,tetcw,az(indm),1.)
  334 continue
      if(pa(inds).ge.0.3) go to 335
      if(pa(inds).le.0.2) go to 335
      call hfill(311,tetcw,0.,1.)
  335 continue
      if(pa(inds).le.0.3) go to 336
      call hfill(411,tetcw,0.,1.)
  336 continue
      call hfill(31,pt(indf),0.,1.)
      call hfill(32,pt(indm),0.,1.)
c      call hfill(32,pt(indf),0.,1.)
      call hfill(33,pt(inds),0.,1.)
      call hfill(34,pt(2),0.,1.)
      call hfill(34,pt(3),0.,1.)
      call hfill(6,pa(1),0.,1.)
      call hfill(7,pa(2),0.,1.)
      call hfill(7,pa(3),0.,1.)
c      call hfill(9,deldd,0.,1.)
      call hfill(10,pa(indf),0.,1.)
c      call hfill(16,pa(1),pa(3),1.)
      go to 11
  100 continue
c   8  format(i9,4(2x,f10.2),/6(2x,f10.5))
      call hidopt (0,'blac')
      call histdo
      call hrput (0,'dpp.his','N')
      stop
      end
