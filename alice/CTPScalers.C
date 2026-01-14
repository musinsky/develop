// 2026-01-14

{
  std::string outd = "/tmp";
  std::string ccf = "20250726.cc"; std::uint32_t run = 564918;
  // std::string ccf = "20241116.cc"; std::uint32_t run = 560089;

  CTPScalers scl;
  std::string s = std::format("ctpc/{}", ccf);
  scl.CCToRoot(s.c_str(), outd.c_str());
  s = std::format("{}/{}.root", outd, ccf);
  // scl.RootToCC(s.c_str(), outd.c_str());
  // $ diff /tmp/20250726.cc.replica <(tail -n +2 ctpc/20250726.cc)
  // $ diff /tmp/20241116.cc.replica <(tail -n +2 ctpc/20241116.cc)
  std::ofstream outf(std::format("{}/classes.conditions.run.{}.txt", outd, run));
  scl.ClassesConditions(s.c_str(), run, outf);
  // scl.ClassesConditions(s.c_str(), run);
}
