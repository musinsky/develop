// 2026-01-22

{
  std::string ind  = "./data_alice_ctp_scalers";
  std::string outd = "/tmp";
  std::string ccf = "20250726.cc"; std::uint32_t run = 564918;
  // std::string ccf = "20241116.cc"; std::uint32_t run = 560089;

  CTPScalers scl;
  std::string in1 = std::format("{}/ctpc/{}", ind, ccf);
  scl.CCToRoot(in1.c_str(), outd.c_str());
  in1 = std::format("{}/{}.root", outd, ccf);
  // scl.RootToCC(in1.c_str(), outd.c_str());
  // $ diff /tmp/20250726.cc.replica <(tail -n +2 ctpc/20250726.cc)
  // $ diff /tmp/20241116.cc.replica <(tail -n +2 ctpc/20241116.cc)

  std::string in2 = std::format("{}/runs", ind);
  std::ofstream conf(std::format("{}/classes.conditions.run.{}.txt", outd, run));
  scl.ClassesConditions(in1.c_str(), run, in2.c_str(), conf);
  // scl.ClassesConditions(in1.c_str(), run, in2.c_str());
}
