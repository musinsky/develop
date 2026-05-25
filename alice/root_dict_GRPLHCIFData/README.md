<!-- markdownlint-disable MD033 MD041-->
<p align="right">last edit: 2026-05-25</p>
<!-- markdownlint-enable  MD033 MD041-->

```plain
$ ./get.O2.files.sh
$ ./compile.custom.dict.sh
$ root test.custom.dict.C
root [0]
Processing test.custom.dict.C...
Fill number:      11745
Injection scheme: 25ns_2460b_2448_2092_2239_144bpi_20inj
Number of bunches: 2092
```

```plain
$ curl http://alice-ccdb.cern.ch/download/651e3112-514a-11f1-b12c-0aa202c71b9a \
       --remote-name --remote-header-name --remote-time
$ stat --format '%n   %y' *
o2-parameters-GRPLHCIFData_1778951539107.root   2026-05-17 00:29:02.000000000 +0200
```
