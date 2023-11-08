-- 2023-10-03

CREATE TABLE DST_files (
  id INTEGER PRIMARY KEY, -- file ID
  name TEXT,              -- file name
  prefix TEXT,            -- prefix path of file
  format TEXT,            -- 'mini' or 'full' format
  real_as_int INTEGER,    -- 1=mischeck int instead of real in DST (0=only real)
  new_name TEXT           -- new file name
);

-- SQLite 3.7.11 (2012-03-20) INSERT syntax allow multiple rows
INSERT INTO DST_files (id, name, prefix, format, real_as_int, new_name)
VALUES
  -- dp 3.35 GeV/c
  (  1, 'all2pr',   'dp',     'mini', 0, 'dp.mini.2pr'        ),
  (  2, 'all4pr',   'dp',     'mini', 0, 'dp.mini.4pr'        ),
  (  3, 'allppn',   'dp',     'mini', 0, 'dp.mini.ppn'        ),
  (  4, 'dp34',     'dp',     'full', 0, 'dp.full.34'         ),
  (  5, 'dp59',     'dp',     'full', 1, 'dp.full.59'         ),
  (  6, 'dp62',     'dp',     'full', 1, 'dp.full.62'         ),
  (  7, 'dp6668',   'dp',     'full', 1, 'dp.full.66.68'      ),
  -- He3p 13.5 GeV/c
  (  8, 'he2prong', 'he3p',   'mini', 0, 'he3p.mini.2pr'      ),
  (  9, 'he3prong', 'he3p',   'mini', 0, 'he3p.mini.3pr'      ),
  ( 10, 'he4prong', 'he3p',   'mini', 0, 'he3p.mini.4pr'      ),
  ( 11, 'he5prong', 'he3p',   'mini', 0, 'he3p.mini.5pr'      ),
  ( 12, 'he6prong', 'he3p',   'mini', 0, 'he3p.mini.6pr'      ),
  ( 13, 'he7prong', 'he3p',   'mini', 0, 'he3p.mini.7pr'      ),
  ( 14, 'he3all',   'he3p',   'full', 0, 'he3p.full.all'      ),
  ( 15, 'he3',      'he3p',   'full', 0, 'he3p.full'          ),
  -- He4p 13.5 GeV/c
  ( 16, 'he13p2',   'he4p13', 'mini', 0, 'he4p-13.mini.2pr'   ),
  ( 17, 'he13p3',   'he4p13', 'mini', 0, 'he4p-13.mini.3pr'   ),
  ( 18, 'he13p4',   'he4p13', 'mini', 0, 'he4p-13.mini.4pr'   ),
  ( 19, 'he13p5',   'he4p13', 'mini', 0, 'he4p-13.mini.5pr'   ),
  ( 20, 'he13p6',   'he4p13', 'mini', 0, 'he4p-13.mini.6pr'   ),
  ( 21, 'he13p7',   'he4p13', 'mini', 0, 'he4p-13.mini.7pr'   ),
  ( 22, 'he13',     'he4p13', 'full', 0, 'he4p-13.full'       ),
  -- He4p 8.6 GeV/c
  ( 23, 'he4prng2', 'he4p8',  'mini', 0, 'he4p-08.mini.2pr'   ),
  ( 24, 'he4prng3', 'he4p8',  'mini', 0, 'he4p-08.mini.3pr'   ),
  ( 25, 'he4prng4', 'he4p8',  'mini', 0, 'he4p-08.mini.4pr'   ),
  ( 26, 'he4prng5', 'he4p8',  'mini', 0, 'he4p-08.mini.5pr'   ),
  ( 27, 'he8p2p4',  'he4p8',  'full', 1, 'he4p-08.full.p2p4'  ),
  ( 28, 'he8p3p5',  'he4p8',  'full', 1, 'he4p-08.full.p3p5'  ),
  ( 29, 'he8pnfit', 'he4p8',  'full', 1, 'he4p-08.full.pnfit' );
