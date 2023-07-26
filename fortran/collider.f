c     from VVG PC
c     $ gfortran collider.f -o collider $(cernlib packlib)
      program  collider
      common // hmemor (10000)
      dimension e(1000),a(100),p(100),b(100),e1(100),e2(100),
     1     t(100),t1(100),t2(100)
      call hlimit (10000)
      open (6,file='kinem.dat')
c     call hbook1(11,'energy$',50.,0.,20.,0.)
c     call hbook2(12,'mom vs energy$',50.,0.,5.,50.,0.,3.,0.)
      call hbook2(13,'mom0 vs kin energy0$',20.,0.,2.,20.,0.,3.5,0.)
      call hbook2(14,'ttri vs kin energy0$',20.,0.,2.,20.,0.,2.,0.)
      call hbook2(15,'tp vs kin energy0$',20.,0.,2.,20.,0.,2.5,0.)
      call hbook2(16,'tp vs ttri$',20.,0.,2.,20.,0.,2.5,0.)
      xn=3.51687428
      xmhe3=2.80841
      xmtri=2.80894
      xm1=xmtri
      xmd=1.87563
      xmp=0.93828
      xm2=xmp
      xmn=0.93957
      e(1)=1.87563
      t(1)=e(1)-xmd
c     c----------------e poroga = 1.87361  -----------
      do 5 i=1,100
         a(i)=4.*e(i)**2-xm1**2-xm2**2
         b(i)=(a(i)**2/4.-(xm1*xm2)**2)
c     write(*,*) b(i)
c     if (b(i).lt.0) b(i)=0.
         p(i)=sqrt(b(i)/(4.*e(i)**2))
         e1(i)=sqrt(p(i)**2+xm1**2)
         t1(i)=e1(i)-xm1
         e2(i)=sqrt(p(i)**2+xm2**2)
         t2(i)=e2(i)-xm2
         e(i+1)=e(i)+0.02
         t(i+1)=t(i)+0.02
c     call hfill(11,e(i),0.,1.)
c     call hfill(12,e(i),p(i),1.)
         call hfill(13,t(i),p(i),1.)
         call hfill(14,t(i),t1(i),1.)
         call hfill(15,t(i),t2(i),1.)
         call hfill(16,t1(i),t2(i),1.)
    5 continue
      write(65,*) xmhe3,e(1),p(1)
c     8 format (7f10.2)
      call hidopt (0,'blac')
      call histdo
      call hrput(0,'kinem.his','N')
      stop
      end
