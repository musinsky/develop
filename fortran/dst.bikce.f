c     from VVG PC: dst/BIKCE.F (2008-12-12)
c     $ gfortran bikce.f $(cernlib packlib)
      program bik
      common // hmemor (10000)
      dimension a(30),pala(4,4),pa(4),ca(4),index(3),pom(4),pla(4,5),
     +     pl(3),cl(3),r(3),xpl(3),ypl(3),b(4,4),c(4,4),xm(3),pt(4),
     +     cosxz(4),tetxz(4),tkin(3)
      data pi/3.141593/
      call hlimit (10000)
      open (11,file='all2pr.dvo',status='old',
     +     form='unformatted')
      open (6,file='bik2pr.dat')
c------------booking of histogram -----------------------------------
      call hbook2 (350,'ps vs t$',50.,0.,0.04,50,0.,0.4,0.)
      call hbook1 (2,'flagpol$',50.,-10.,10.,0.)
c       call hbook1 (4,'q=efmpp-mn-mp,tsma 0.1,$',50.,0.,0.1,0.)
c       call hbook1 (5,'q=efmpp-mn-mp$',50.,0.0006,0.0007,0.)
c       call hbook1 (6,'q=efmpp-mn-mp$',50.,0.0007,0.0008,0.)
c       call hbook1 (7,'q=efmpp-mn-mp$',50.,0.0008,0.0009,0.)
c       call hbook1 (8,'q=efmpp-mn-mp$',50.,0.0009,0.0010,0.)
c       call hbook2 (25,'S/SEL  vs efmsq$',50.,0.,0.5,20.,0.5,1.5,0.)
c       call hbook2 (26,'S/SELas  vs efmsq$',50.,0.,0.04,20.,0.5,1.5,0.)
c       call hbook1 (1,'bifm+biifm-biii$',50.,-0.5,0.5,0.)
c       call hbook1 (2,'biii-bi_s$',50.,-0.5,0.5,0.)
c       call hbook1 (3,'biifm+bifm-bi_s$',50.,-0.5,0.5,0.)
c      call hbook2 (19, 'p n L-sy vs tet n all$',50,0.,100.,50.,0.,2.,0.)
c      call hbook2 (48, 'tetxz vs mom of p ce$',50,0.,4.,50.,-90.,90.,0.)
c      call hbook2 (49, 'tetxz vs mom of p $',50,0.,4.,50.,-90.,90.,0.)
c       call hbook2 (5,'fis vs fim$',10.,0.,3.,10.,0.,3.,0.)
c       call hbook2 (5,'mom of s vs mom of fast$',50.,1.,2.,50.,1.,2.,0.)
c       call hbook1 (9,'mom of s+m for small angle$',50.,0.,4.,0.)
      call hbook1 (6,'-t (angl p1,p2 sma 3 deg)$',10.,0.,0.02,0.)
c       call hbook1 (7,'-t (angl p1,p2 sma 3 deg)$',4.,0.,0.004,0.)
c       call hbook1 (8,'-t (angl p1,p2 sma 3 deg)$',50.,0.,0.05,0.)
c       call hbook1 (27,'-t (angl p1,p2 sma 1.5 deg)$',4.,0.,0.004,0.)
c       call hbook1 (29,'-t (angl p1,p2 sma 2 deg)$',4.,0.,0.004,0.)
c       call hbook1 (30,'-t (angl p1,p2 sma 2 deg)$',50.,0.,0.05,0.)
c       call hbook1 (230,'tkin n (ang p1,p2 sma 2 deg)$',50.,0.,0.02,0.)
c       call hbook1 (31,'-t (angl p1,p2 sma 2.5 deg)$',4.,0.,0.004,0.)
c       call hbook1 (33,'-t (angl p1,p2 sma 3.5 deg)$',4.,0.,0.004,0.)
c       call hbook1 (35,'-t (angl p1,p2 sma 4 deg)$',4.,0.,0.004,0.)
c       call hbook1 (36,'-t (angl p1,p2 sma 4 deg)$',50.,0.,0.05,0.)
c       call hbook1 (37,'-t (angl p1,p2 sma 4.5 deg)$',4.,0.,0.004,0.)
c       call hbook1 (39,'-t (angl p1,p2 sma 5. deg)$',4.,0.,0.004,0.)
c       call hbook1 (40,'-t (angl p1,p2 sma 5. deg)$',50.,0.,0.05,0.)
c       call hbook1 (41,'-t (angl p1,p2 sma 5.5 deg)$',4.,0.,0.004,0.)
c       call hbook1 (43,'-t (angl p1,p2 sma 1. deg)$',4.,0.,0.004,0.)
c       call hbook1 (45,'-t (angl p1,p2 sma 1.5 deg)$',4.,0.,0.004,0.)
c       call hbook1 (8,'t $',50.,0.,0.04,0.)
c       call hbook1 (10,'deltap two prot, small angle$',50.,0.,0.1,0.)
c       call hbook1 (11,'mom of spec in lab$',50.,0.,4.,0.)
c       call hbook1 (12,'mom of s d sys$',50.,0.,1.2,0.)
c       call hbook1 (13,'mom of s d sys,tetsma5d$',50.,0.,1.2,0.)
c       call hbook1 (113,'mom of s lab sys,tetsma5d$',50.,1.,2.,0.)
c       call hbook1 (14,'mom of s d sys,tetsma4d$',50.,0.,1.2,0.)
c       call hbook1 (15,'mom of s d sys,tetsma3,5d$',50.,0.,1.2,0.)
c       call hbook1 (16,'mom of s d sys,tetsma3d$',50.,0.,1.2,0.)
c       call hbook1 (216,'kin en of n lab sys,tetsma3d$',50.,0.,1.,0.)
c       call hbook1 (17,'mom of s d sys,tetsma2.5d$',50.,0.,1.2,0.)
c       call hbook1 (18,'mom of s d sys,tetsma2d$',50.,0.,1.2,0.)
c       call hbook1 (19,'mom of s d sys,tetsma1.5d$',50.,0.,1.2,0.)
c       call hbook1 (119,'mom of s lab sys,tetsma1.5d$',50.,1.,2.,0.)
c       call hbook1 (20,'mom of s d sys,tetsma1d$',50.,0.,1.2,0.)
c       call hbook1 (120,'mom of s lab sys,tetsma1d$',50.,1.,2.,0.)
c       call hbook1 (22,'mom of s d sys,tetsma0.5d$',50.,0.,1.2,0.)
c       call hbook1 (122,'mom of s lab sys,tetsma0.5d$',50.,1.,2.,0.)
c       call hbook1 (213,'kin energ of n lab sys$',50.,0.,2.,0.)
c       call hbook1 (14,'mom s dsys tetlab sma 3 deg$',50.,0.,0.5,0.)
c       call hbook1 (15,'mom m dsys tetlab sma 3 deg$',50.,0.,0.5,0.)
c       call hbook1 (20,'mom s dsys tetlab sma 2 deg$',50.,0.,0.5,0.)
c       call hbook1 (21,'mom m dsys tetlab sma 2 deg$',50.,0.,0.5,0.)
c       call hbook1 (22,'mom s dsys tetlab sma 1 deg$',50.,0.,0.5,0.)
c       call hbook1 (23,'mom m dsys tetlab sma 1 deg$',50.,0.,0.5,0.)
c       call hbook1 (16,'costet s  labsys $',25.,0.999,1.,0.)
c       call hbook1 (17,'costet m  labsys $',25.,0.999,1.,0.)
c       call hbook1 (18,'tet s  labsys $',50.,0.,3.,0.)
c       call hbook1 (19,'tet m  labsys $',50.,0.,3.,0.)
c       call hbook1 (25,'pt of spectators$',50.,0.,0.5,0.)
c       call hbook1 (16,'sum py$',50.,-1.,1.,0.)
c       call hbook1 (17,'sum pz$',50.,-1.,1.,0.)
c       call hbook1 (18,'mom of scatter$',50.,1.2,1.7,0.)
c       call hbook1 (20,'efm scat + rec$',50.,1.8,2.5,0.)
c       call hbook1 (14,'mass of all$',50.,0.,2.,0.)
c       call hbook1 (7,'efmpp for small angle$',50.,1.85,2.05,0.)
c       call hbook1 (5,'ymu$',50.,0.92,0.96,0.)
c       call hbook1 (6,'biifm+bifm-bi_s2$',50.,-0.5,0.5,0.)
c       call hbook1 (7,'b1_s-b12$',50.,-0.3,0.,0.)
c       call hbook1 (8,'b1_s2-b12$',50.,-0.3,0.,0.)
c---------------------------------------------------------------------
      n=0
   11 read (11,end=100) nw, (a(i), i=1,nw)
      n=n+1
c      write(*,*) n
c      iflag=a(1)
c      if(a(4).lt.-1.) go to 11
      nexp=a(2)/1000.
      nexp=a(2)-nexp*1000.
c------------------------------beam--------------------------
      call hfill(2,a(1),0.,1.)
      amp=0.93827231
      amn=0.93956563
      amd=1.87563
      pala(1,4)=a(6)
      pla(1,4)=a(5)
      pala(2,4)=a(8)
      pla(2,4)=a(8)
      pala(3,4)=a(9)
      pla(3,4)=a(9)
      pala(4,4)=sqrt(a(6)**2+amp**2)
      pla(4,4)=sqrt(a(5)**2+a(10)**2)
c------------------------------target-------------------------
      pla(1,5)=0.
      pla(2,5)=0.
      pla(3,5)=0.
      pla(4,5)=amp
      do 7 l=1,4
         b(l,4)=pala(l,4)/amp
    7    c(l,4)=pla(l,4)/a(10)
c--------------------------end of beam-------------------------
      do 2 j=1,3
         ind=j*6+5
         pala(1,j)=a(ind+1)
         pla(1,j)=a(ind)
         pala(2,j)=a(ind+3)
         pla(2,j)=a(ind+3)
         pala(3,j)=a(ind+4)
         pla(3,j)=a(ind+4)
         pa(j)=sqrt(pala(1,j)**2+pala(2,j)**2+pala(3,j)**2)
         pl(j)=sqrt(pla(1,j)**2+pla(2,j)**2+pla(3,j)**2)
         pala(4,j)=sqrt(pa(j)**2+a(ind+5)**2)
         pla(4,j)=sqrt(pl(j)**2+a(ind+5)**2)
         tkin(j)=pla(4,j)-a(ind+5)
         pt(j)=sqrt(pla(2,j)**2+pla(3,j)**2)
         cosxz(j)=pla(1,j)/sqrt(pla(1,j)**2+pla(2,j)**2)
         if(cosxz(j).gt.1.)go to 2
         tetxz(j)=acos(cosxz(j))*180./pi
         if(pla(2,j).lt.0.) tetxz(j)=-tetxz(j)
         xm(j)=a(ind+5)
         do 9 k=1,4
            b(k,j)=pala(k,j)/xm(j)
    9       c(k,j)=pla(k,j)/xm(j)
    2 continue
      call hfill(49,pl(1),tetxz(1),1.)
      call hfill(49,pl(2),tetxz(2),1.)
c---------------------sort in ala system------------------------------
      call sortzv (pa,index,3,1,1,0)
      indf=index(1)
      indm=index(2)
      inds=index(3)
c---------------------select cex channel------------------------------
      if (indf.ne.3) go to 11
c---------------------------------------------------------------------
      call hfill(12,pa(inds),0.,1.)
      if(fis.ge.5.) go to 101
c      call hfill(13,pa(inds),0.,1.)
c      call hfill(113,pl(inds),0.,1.)
c      call hfill(213,tkin(3),0.,1.)
  101 continue
      if(fis.ge.4.) go to 102
c      call hfill(14,pa(inds),0.,1.)
  102 continue
      if(fis.ge.3.5) go to 103
c      call hfill(15,pa(inds),0.,1.)
  103 continue
      if(fis.ge.3.) go to 104
c      call hfill(16,pa(inds),0.,1.)
c      call hfill(216,tkin(3),0.,1.)
  104 continue
      if(fis.ge.2.5) go to 105
c      call hfill(17,pa(inds),0.,1.)
  105 continue
      if(fis.ge.2.) go to 106
c      call hfill(18,pa(inds),0.,1.)
  106 continue
      if(fis.ge.1.5) go to 107
c      call hfill(19,pa(inds),0.,1.)
c      call hfill(119,pl(inds),0.,1.)
  107 continue
      if(fis.ge.1.) go to 108
c      call hfill(20,pa(inds),0.,1.)
c      call hfill(120,pl(inds),0.,1.)
  108 continue
      if(fis.ge.0.5) go to 109
c      call hfill(22,pa(inds),0.,1.)
c      call hfill(122,pl(inds),0.,1.)
  109 continue
c      call hfill(14,xm(3),0.,1.)
c      call hfill(12,a(5),0.,1.)
c      call hfill(11,pl(inds),0.,1.)
c       if (pt(indm).lt.0.621) go to 45
c       if (pt(indm).gt.0.623) go to 45
c      write(6,fmt=8) nw,(a(i),i=1,nw)
c   45 continue
c       if(pa(inds).lt.0.5) go to 11
c      call hfill(13,pt(indm),0.,1.)
c      call hfill(13,pt(indf),0.,1.)
c       call hfill(25,pt(inds),0.,1.)
c      call hfill(18,pa(indm),0.,1.)
c      call hfill(18,pa(indf),0.,1.)
c      call hfill(48,pl(1),tetxz(1),1.)
c      call hfill(48,pl(2),tetxz(2),1.)
      spy=pla(2,1)+pla(2,2)+pla(2,3)
      spz=pla(3,1)+pla(3,2)+pla(3,3)
c      call hfill(16,spy,0.,1.)
c      call hfill(17,spz,0.,1.)
      call vadd(pala(1,1),pala(1,2),pom,4)
      efmpp=sqrt(abs(doti(pom,pom)))
      call vadd(pala(1,indm),pala(1,indf),pom,4)
      efmpn=sqrt(abs(doti(pom,pom)))
c      call hfill(20,efmpn,0.,1.)
      csfi1=pla(1,1)/pl(1)
      csfi2=pla(1,2)/pl(2)
      csfis=pla(1,inds)/pl(inds)
      csfim=pla(1,indm)/pl(indm)
      fi1=acos(csfi1)
      fi2=acos(csfi2)
      fis=acos(csfis)
      fim=acos(csfim)
      fi1=fi1*180./pi
      fis=fis*180./pi
c      if(pla(2,1).lt.0.) fi1=-fi1
      fi2=fi2*180./pi
      fim=fim*180./pi
c      if(pla(2,2).lt.0.) fi2=-fi2
      if(abs(fi1).gt.3..or.abs(fi2).gt.3.) go to 66
      if(pl(1).lt.1.) go to 66
      if(pl(1).gt.2.) go to 66
      if(pl(2).lt.1.) go to 66
      if(pl(2).gt.2.) go to 66
c      call hfill(5,fi1,fi2,1.)
c      call hfill(5,pl(indm),pl(inds),1.)
c      call hfill(7,efmpp,0.,1.)
   66 continue
c-------------------- calcul and select t ----------------------------
      call vsub (pla(1,5),pla(1,3),pom,4)
      t=abs(doti(pom,pom))
      if(pa(inds).gt.0.1) go to 33
      if(t.gt.0.1) go to 33
      call hfill(4,q,0.,1.)
   33 continue
      q=efmpp-2*amp
c      call hfill(5,q,0.,1.)
c      call hfill(6,q,0.,1.)
c      call hfill(7,q,0.,1.)
c      call hfill(8,q,0.,1.)
c      if(pa(inds).gt.0.2) go to 44
c      call hfill(8,t,0.,1.)
      call hfill(350,t,pa(inds),1.)
c   44 continue
c      wes=6.2/(17524.*0.001)
      if(fi1.ge.3.) go to 77
      if(fi2.ge.3.) go to 77
      wes=6.2/(17524.*0.002)
      call hfill(6,t,0.,wes)
      wes=6.2/(17524.*0.001)
c      call hfill(7,t,0.,wes)
c       call hfill(6,t,0.,1.)
c       call hfill(7,t,0.,1.)
c      call hfill(8,t,0.,1.)
      call vsub(pla(1,1),pla(1,2),pom,3)
      xdelp=abs(doti(pom,pom))
c       call hfill(14,pa(inds),0.,1.)
c       call hfill(15,pa(indm),0.,1.)
c       call hfill(16,csfis,0.,1.)
c       call hfill(17,csfim,0.,1.)
c       call hfill(18,fis,0.,1.)
c       call hfill(19,fim,0.,1.)
c      call hfill(9,pl(indm),0.,1.)
c      call hfill(9,pl(inds),0.,1.)
c      call hfill(13,pa(inds),0.,1.)
c      call hfill(13,pa(indm),0.,1.)
c      call hfill(10,xdelp,0.,1.)
   77 continue
      if(fi1.ge.1.5) go to 88
      if(fi2.ge.1.5) go to 88
c      wes=6.2/(17524.*0.002)
c      call hfill(27,t,0.,wes)
   88 continue
      if(fi1.ge.2.) go to 99
      if(fi2.ge.2.) go to 99
c      wes=6.2/(17524.*0.002)
c      call hfill(29,t,0.,wes)
c      call hfill(30,t,0.,1.)
c      call hfill(230,t,0.,1.)
   99 continue
      if(fi1.ge.2.5) go to 89
      if(fi2.ge.2.5) go to 89
c      wes=6.2/(17524.*0.002)
c      call hfill(31,t,0.,wes)
   89 continue
      if(fi1.ge.3.5) go to 90
      if(fi2.ge.3.5) go to 90
c      wes=6.2/(17524.*0.002)
c      call hfill(33,t,0.,wes)
   90 continue
      if(fi1.ge.4.) go to 91
      if(fi2.ge.4.) go to 91
c      wes=6.2/(17524.*0.002)
c      call hfill(35,t,0.,wes)
c      call hfill(36,t,0.,1.)
   91 continue
      if(fi1.ge.4.5) go to 92
      if(fi2.ge.4.5) go to 92
c      wes=6.2/(17524.*0.002)
c      call hfill(37,t,0.,wes)
   92 continue
      if(fi1.ge.5.) go to 93
      if(fi2.ge.5.) go to 93
c      wes=6.2/(17524.*0.002)
c      call hfill(39,t,0.,1.)
c      call hfill(40,t,0.,1.)
   93 continue
      if(fi1.ge.5.5) go to 94
      if(fi2.ge.5.5) go to 94
c      wes=6.2/(17524.*0.002)
c      call hfill(41,t,0.,1.)
   94 continue
      if(fi1.ge.1.) go to 95
      if(fi2.ge.1.) go to 95
c      wes=6.2/(17524.*0.002)
c      call hfill(43,t,0.,1.)
   95 continue
      if(fi1.ge.1.5) go to 96
      if(fi2.ge.1.5) go to 96
c      wes=6.2/(17524.*0.002)
c      call hfill(45,t,0.,1.)
   96 continue
c      if (t.lt.0.04) go to 11
c---------------------calcul and select bik---------------------------
c      if(pa(inds).gt.0.25) go to 11
      call vsub(c(1,4),c(1,inds),pom,4)
      bII1=abs(doti(pom,pom))
      call vadd(pala(1,indf),pala(1,indm),pom,4)
      efms=abs(doti(pom,pom))
      gami=pala(4,4)/amp
      sel=2.*gami*xm(indm)*xm(indf)+xm(indm)**2+xm(indf)**2
      selas=2.*amp**2+2.*pala(4,4)*amp
      efms=efms/sel
      efmsas=efms*sel/selas
c      call hfill(25,bII1,efms,1.)
c      call hfill(26,bII1,efmsas,1.)
      go to 11
c      write(*,*) n
  100 continue
    8 format(i9,4(2x,f10.2),/6(2x,f10.5))
      call hidopt (0,'blac')
      call histdo
      call hrput (0,'bik2pr.his','N')
      stop
      end
