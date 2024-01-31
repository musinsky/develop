C 2004-10-18
C https://www.phy.bnl.gov/e949/detector/tracking/utc/utc_align.html

      LOGICAL FUNCTION COSMIC_LINFIT(NPTS,LFIT,X,Y,RESLN,
     &     P,CHI,NDOF,V,RESMAX,IRESMAX,RESID,XTRAER)
C---------------------------------------------------------------------------
C-
C-   Purpose and Methods :
C      Special version for fitting cosmics to RSSC points.  Z cut
C       at 50cm removed. Added calling argument RESLN.
C-
C-   Returned value  :
C-   Inputs  :
C-   Outputs :
C-   Controls:
C-   Description:  Y = A * X + B
C                  The error matrix V is V(1,1)=sigma_aa
C                                        V(1,2)=sigma_ab
C                                        V(2,2)=sigma_bb
C
C-
C-   Created  14-OCT-1990   Yoshitaka Kuno
C-            15-Dec-1993   (GR) Modified name from LINEFIT to LINFIT
C-                                to avoid conflict with LINEFIT in
C-                                KOFIA_DC
C             06-Apr-1994   (GR) Added calling argument LFIT.
c                                if true, then include the hit in the
c                                 fit.  if false, don<t include the hit
C                                 in the fit, but calculate a residual
C                                 for it.
C             08-Sep-1994   (GR) Return chisq and ndof separately
C                                 rather than chisq/ndof
C                                Calculate chisq only with those hits
c                                 included in the fit!!!
C             28-Apr-1995   (GR) Added return argument XTRAER which
C                                 gives the extrapolation error of
C                                 the line at each point.
C-
C----------------------------------------------------------------------
      IMPLICIT NONE
#include <luns.cmn>
#include <info.cmn>
#include <utc_geom.cmn>
c
c constant
c
      INTEGER NPARAM
      PARAMETER( NPARAM = 2)
c
c input/output variables
c
      REAL    X(*), Y(*), P(NPARAM), CHI, V(NPARAM,NPARAM)
      REAL RESID(*), XTRAER(*), BLAH,RESLN
      LOGICAL LFIT(*)
      INTEGER NPTS,NDOF
c
c local variable
c
      REAL    DSQ, VAL, RESMAX
      INTEGER I, IPZ, IP, Kount, IRESMAX, NBAD, NUSED
      REAL*8  W, RES
      REAL*8  XX, YY, XS, YS, X2S, XYS, A, B, S
C
      SAVE
C
      RES = RESLN

      COSMIC_LINFIT = .FALSE.

      NUSED=0
      DO I=1,NPTS
         IF(LFIT(I)) NUSED=NUSED+1
      END DO

      IF( NUSED .LT. 2 ) GOTO 9999
C
      S   = 0.0
      XS  = 0.0
      YS  = 0.0
      X2S = 0.0
      XYS = 0.0
      NBAD = 0
      DO 1000 I = 1, NPTS
         IF(LFIT(I)) THEN
            XX  = DBLE( X(I) )
            YY  = DBLE( Y(I) )
c          IF(YY.LT.-50)THEN
c            WRITE(LLOG,*)' Run ',NRUN,' Event ',NEVT,' TROUBLE Z=',YY
c            WRITE(LOUT,*)' Run ',NRUN,' Event ',NEVT,' TROUBLE Z=',YY
c            NBAD = NBAD+1
c            GO TO 1000
c          ENDIF
            W = 1.0 / RES**2
            S   = S   + 1.0 * W
            XS  = XS  + XX  * W
            YS  = YS  + YY  * W
            X2S = X2S + XX * XX * W
            XYS = XYS + XX * YY * W
         ENDIF
 1000 CONTINUE
c
c     now calculate A and B
c
      VAL = S*X2S - XS*XS
      IF( VAL .EQ. 0 ) GOTO 9999
      A = ( S*XYS - XS*YS ) / VAL
      B = ( X2S*YS - XS*XYS ) / VAL
      P(1) = REAL( A )
      P(2) = REAL( B )
c
c evaluate the residual and chisq
c
      CHI = 0.0
      RESMAX = 0.0
      DO I = 1, NPTS
c        IF(Y(I).GT.-50.) THEN
         DSQ = Y(I) - P(1)*X(I) - P(2)
         IF(LFIT(I)) CHI = CHI + DSQ*DSQ*W
         RESID(I) = DSQ
         IF(abs(DSQ).GT.abs(RESMAX))THEN
            RESMAX=DSQ
            IRESMAX = I
         ENDIF
c        ENDIF
      ENDDO

      NDOF = NUSED-NPARAM
c
c error matrix
c
      V(1,1) = S/VAL
      V(1,2) = -XS/VAL
      V(2,2) = X2S/VAL
      V(2,1) = V(1,2)

C-- Extrapolation error

      DO I=1,NPTS
         BLAH = X(I)**2*V(1,1) + V(2,2) + 2.0*X(I)*V(1,2)
         IF(BLAH.GT.0.) THEN
            XTRAER(I) = SQRT(BLAH)
         ELSE
            XTRAER(I) = -1.
         ENDIF
      END DO

      COSMIC_LINFIT = .true.
c
 9999 RETURN
      END
