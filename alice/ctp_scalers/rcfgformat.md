# .rcfg format

## run2 format
 
1. BCMASKS section defines **all** masks used later in CLASSES section:
2. CLASSES section: **sym. BCmask** name used in classname but **raw** name used
 also later in the same line. 1st {NONE} is always there (P/F) ?

```
BCMASKS:
BCM4 1264H1L84H1L2214H
NONE

CLASSES:
C0AMU-ABCE-NOPF-CENTNOTRD 50 D0AMU CENTNOTRD {NONE} {NONE} 0 0 0 0 50
CADAND-E-NOPF-CENTNOTRD 31 DADAND CENTNOTRD {NONE} {BCM4} 2097151 0 0 0 31
```

## run3 format without first VER line
```
see DBCTP/pd/all.pd

run 530953
bcd10 7
bcd20 40077
LTG emc
cal ppcal 2953 3012 4000 356399999
LTG ft0
0 cluster cluemc emc  
0 cl_bcd bcd20   
1 cluster clu4 hmp  
1 clu_hmp bcd10   
```

## run3 format from march 2023 ( VER line at the start (.rcfg2))
```
VER 1
bcd10 7
bcd20 40077
LTG emc
cal ppcal 2953 3012 4000 356399999
LTG ft0
0 cluster cluemc emc  
#0 cl_bcd bcd20   
CTRUE-ABCE-NOPF-cluemc 1 DTRUE_bcd20 cluemc {NONE} {NONE} 0
1 cluster clu4 hmp  
#1 clu_hmp bcd10  
CTRUE-ABCE-NOPF-clu4 2 DTRUE_bcd10 clu4 {NONE} {NONE} 0
```
alebo:
```
run 70
BCMASKS:
BCM1 3563H1L         # bcmasks.cfg: bcmB 3563H1L
BCM2 3562H2L         # .pd: bcm bcmPH 3563 3564
0 cluster clu2 hmp  
#0 mb_B   bcmB 
CTRUE-B-NOPF-clu2 1 DTRUE clu2 {NONE} {BCM1} 0

#1 mb_Brnd rnd10  bcmB 
CTRUE-B-NOPF-clu2 2 DTRUE_rnd10 clu2 {NONE} {BCM1} 0

#2 mb_Bdsc   bcmB 0x1ff7ce
CTRUE-B-NOPF-clu2 3 DTRUE clu2 {NONE} {BCM1} 0x1ff7ce

#3 tvx  103 bcmPH 
CMTVX-PH-NOPF-clu2 4 DMTVX clu2 {NONE} {BCM2} 0
```

differencies:
- BCMASKS section with physically allocated bcmasks
    NONE line present if there is a class without bcmask
##r2:
```
BCMASKS:
#cs 25ns_1868b_1866_1089_1749_128bpi_17i8b4e
BCM1 29H1L4L7...
NONE
CLASSES:
CCUP13-B-nOPF-CENTNOTRD 7 DCUP13 CENTNOTRD {NOPF} {BCM1} 2086665 0 0 0 7
```

##r3 ver0 (till 3/2023):
```
bcm bcmB 29H1L4L7...
2 mb_Bdsc DCUP13  bcmB
```

##r3 ver1 (from 4/2023):
```
bcm bcmB 29H1L4L7...
0 cluster CENTNOTRD ...
CCUP13-B-NOPF-CENTNOTRD 3 DCUP13 CENTNOTRD {NOPF} {bcmB} 0
```

- INPUTS section: inputs +  rnd/bcd gens

- class line: instead of input number, descriptor name provided (2 times )
  descriptor name extended (e.g. _rnd1m)
- class numbers are +1
- .rcfg can be created only after bcm allocation i.e. 
  PH (in class name) but also BCM2 (allocated) + BCMASKS section

