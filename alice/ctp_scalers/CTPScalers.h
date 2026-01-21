// Date:   2025-11-19
// Author: Musinsky Jan

#ifndef CTP_Scalers
#define CTP_Scalers

// AliceO2/DataFormats/Detectors/CTP/include/DataFormatsCTP/Scalers.h
static constexpr std::uint32_t NCOUNTERS = 1085;
static constexpr std::uint64_t LIMIT32 = 0xFFFFFFFFull; // = UINT32_MAX
enum EScalerOffset { // TTree::Draw() needs as global enum (and not class) declaration
  kInput = 599,     // [599 - 646] (48 inputs)
  kLMB = 647,       // [647 - 710] (64 LMB classes)
  kLMA = kLMB + 64, // [711 - 774] (64 LMA classes)
  kL0B = kLMA + 64, // [775 - 838] (64 L0B classes)
  kL0A = kL0B + 64, // [839 - 902] (64 L0A classes)
  kL1B = kL0A + 64, // [903 - 966] (64 L1B classes)
  kL1A = kL1B + 64  // [967 -1030] (64 L1A classes)
};

class CTPScalerEntry : public TObject {

public:
  enum class EState : std::uint8_t {
    kNonDecreasing = 1, // correct
    kOverflow      = 2, // correct
    kDecreasing    = 3  // incorrect
  };

  CTPScalerEntry();
  ~CTPScalerEntry() override;

  std::uint64_t GetEpoch() const { return fEpoch; }
  double GetEpochSec() const { return fEpoch/1000000000.; }
  std::uint64_t GetEpochIncr() const { return fEpochIncr; }
  double GetEpochIncrSec() const { return fEpochIncr/1000000000.; }
  const std::array<std::uint32_t, NCOUNTERS>& GetScalers() const { return fScalers; }
  std::uint32_t GetScaler(std::size_t idx) const { return fScalers.at(idx); }
  std::uint32_t GetScaler(EScalerOffset ofs, std::size_t idx) const {
    return fScalers.at(ofs + idx); }
  const std::array<std::uint32_t, NCOUNTERS>& GetScalersIncr() const { return fScalersIncr; }
  std::uint32_t GetScalerIncr(std::size_t idx) const { return fScalersIncr.at(idx); }
  std::uint32_t GetScalerIncr(EScalerOffset ofs, std::size_t idx) const {
    return fScalersIncr.at(ofs + idx); }
  const std::array<std::uint8_t, NCOUNTERS>& GetScalersStat() const { return fScalersStat; }
  std::uint8_t  GetScalerStat(std::size_t idx) const { return fScalersStat.at(idx); }
  std::uint8_t  GetScalerStat(EScalerOffset ofs, std::size_t idx) const {
    return fScalersStat.at(ofs + idx); }

  void  SetEpoch(std::uint64_t e) { fEpoch = e; }
  void  SetEpochIncr(std::uint64_t ei) { fEpochIncr = ei; }
  void  AssignScalers(const std::array<std::uint32_t, NCOUNTERS>& s) { fScalers = s; }
  void  AssignScalersIncr(const std::array<std::uint32_t, NCOUNTERS>& si) { fScalersIncr = si; }
  void  AssignScalersStat(const std::array<std::uint8_t, NCOUNTERS>& ss) { fScalersStat = ss; }

  void  CheckScalers(const CTPScalerEntry* prev);
  bool  HasRun(std::uint32_t run) const;

private:
  std::uint64_t fEpoch = 0;     // epoch time (nanoseconds precision)
  std::uint64_t fEpochIncr = 0; // increment of epoch time
  std::array<std::uint32_t, NCOUNTERS> fScalers = {};     // scalers/counters value
  std::array<std::uint32_t, NCOUNTERS> fScalersIncr = {}; // increment of scalers value
  std::array<std::uint8_t, NCOUNTERS> fScalersStat = {};  // state of scalers value
  // https://github.com/root-project/root/issues/20181
  // std::array<EState, NCOUNTERS> fScalersStat2 = {};

  ClassDefOverride(CTPScalerEntry, 1) // CTP Scaler Entry
};

class CTPScalers : public TObject {

public:
  CTPScalers() = default;
  ~CTPScalers() override = default;

  static constexpr const char* CCTN = "cc"; // CC (CTP counters) Tree Name
  std::uint64_t QuickStrToULL(const char* nptr) const;
  void  CCToRoot(const char* ccfname, const char* outdn = ".", bool skipFirst = true) const;
  void  RootToCC(const char* rootfn, const char* outdn = "/tmp") const;
  std::vector<std::size_t> ParseActiveClasses(std::uint32_t run,
                                              const char* cfgdir = "runs") const;
  void  ClassesConditions(const char* rootfn, std::uint32_t run,
                          std::ostream& stream = std::cout) const;

private:

  ClassDefOverride(CTPScalers, 0) // CTP Scalers
};

#endif
