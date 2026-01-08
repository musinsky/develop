// 2026-01-08

{
  CTPScalers scl;
  scl.CCToRoot("ctpc/20250726.cc", "/tmp");
  // scl.RootToCC("/tmp/20250726.cc.root", "/tmp");
  // // $ diff /tmp/20250726.cc.replica <(tail -n +2 ctpc/20250726.cc)
  std::ofstream outf("/tmp/20250726.cc.ClassesConditions.txt");
  scl.ClassesConditions("/tmp/20250726.cc.root", 564918, outf);
  // scl.ClassesConditions("/tmp/20250726.cc.root", 564918);
}
