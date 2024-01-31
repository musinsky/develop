c     2005-10-27
c     2019-05-14
c
c     output:
c     1.69238281   3.47169137   0.295643777   1369.84827   151.067001
c
      program magnet_vvg
c     ( B {tesla] )
      hfh=0.847845
c     ( moje -x )
      ybs=-2.80654
c     ( moje pz )
      px=3.48055
c     ( moje -px)
      py=-0.08843
c     ( moje p_trans)
      p=3.484257
c     phias=acos(px/p)
      phias=asin(py/p)
c-----------uslovija prokhoda s cherez magnit-------------------------
      RHSH=P/(0.003*HFH)
      YFS=YBS+RHSH*COS(PHIAS)-SQRT(RHSH**2-(151.+RHSH*SIN(PHIAS))**2)
      SHS=SQRT((151.)**2+(YFS-YBS)**2)
      zz=shs/(2.*rhsh)
      if(zz.gt.1.) zz=1.
      PHIFS=2.*ASIN(zz)+PHIAS
      pxf=p*cos(phifs)
      pyf=p*sin(phifs)
      write(*,*) yfs,pxf,pyf,rhsh, shs
      END
