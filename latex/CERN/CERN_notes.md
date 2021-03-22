Recommendations for editing
---------------------------
Last update 2021-03.

## CTAN packages
Snazit sa vyuzivat uz hotove baliky (a neobjavovat znova koleso), ktore su prehladne zoskupene v roznych kolekciach, napr. [Scientific docs](https://ctan.org/topic/scientific-docs), [Physics](https://ctan.org/topic/physics) alebo [HEP (by Andy Buckley)](https://ctan.org/author/buckley). Taketo baliky znacne ulahcuju zivot a hlavne dodrziavaju **typograficke pravidla** pre sadzbu dokumentov. Dobrym prikladom su napr.:
* [siunitx](https://ctan.org/pkg/siunitx) – A comprehensive (SI) units package
* [hepunits](https://ctan.org/pkg/hepunits) – A set of units useful in high energy physics applications

## CERN
[CERN Authoring](https://authoring.web.cern.ch/) od 2018-01 ponuka pre vsetkych CERN users [Overleaf](https://www.overleaf.com/org/cern) **free** [Professional account](https://www.overleaf.com/user/subscription/plans).

[CERN Publishing Service](https://information-technology.web.cern.ch/services/publishing-service) poskytuje zakladne info ohladom vytvorenia vedeckeho textu. CERN Reports (also called Yellow Reports) [template files](https://publishing-files.web.cern.ch/publishing-files/templates/CYR/LaTeX/) takisto **pouziva** rozne uz hotove **CTAN packages** (`hep*.sty` by Andy Buckley), pritom znacne zastaralych verzii. Rozne prirucky a usmernenia (recommended abbreviations, spelling list, ...) sa nachadzaju [tu](https://publishing-files.web.cern.ch/publishing-files/Userguide/).

## CERN ALICE
* https://twiki.cern.ch/twiki/bin/view/ALICE/EditorialBoard
* https://twiki.cern.ch/twiki/bin/view/ALICE/GuidelineEditing

### ALICE Publications
* https://alice-publications.web.cern.ch
* https://alice-publications.web.cern.ch/Documents/drafts/tools.html

### ALICE Notes
* https://alice-notes.web.cern.ch/

Ak nepotrebujem vytvorit CERN/ALICE dokument, tak zmysel ma vyuzit len definicie (LaTeX commands) pre typograficky korektne pisanie roznych HEP jednotiek, nazvov castic a pod. alebo ALICE standardov.
* `cernunits.sty` from `cernrep.zip` (CERN Publishing Service, LaTeX commands)
* `cernrep.{tex,pdf}` from `cernrep.zip` (CERN Publishing Service, general recommendations)
* `commands.tex` from `CDS_templates-2018.zip` (ALICE Publications tools, LaTeX commands)

Ale aj tu vidiet nejednotnost v LaTeX-ovani, napr. GeV/c je definovane rozne ?!

## Summarum
Pouzivat uz hotove CTAN baliky. Napr. pre pisanie jednotiek je kombinacia `siunitx` a `hepunits` CTAN balikov pravdepodobne najlepsia volba.
