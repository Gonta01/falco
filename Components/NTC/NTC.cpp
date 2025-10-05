// ======================================================================
// \title  NTC.cpp
// \author user
// \brief  cpp file for NTC component implementation class
// ======================================================================

#include "Components/NTC/NTC.hpp"
#include "FpConfig.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  NTC ::
    NTC(const char* const compName) :
      NTCComponentBase(compName)
  {

  }

  NTC ::
    ~NTC()
  {

  }

  void NTC::configure(F32 k, F32 b) {
    calibrCoeffK = k;
    calibrCoeffB = b;
  }

using NTCTableValue = NTC::NTCTableValue;
constexpr auto ntcTable = etl::make_array<NTCTableValue>(
        NTCTableValue{-55, 636177}, NTCTableValue{-54, 594705}, NTCTableValue{-53, 556217}, NTCTableValue{-52, 520480},
        NTCTableValue{-51, 487280}, NTCTableValue{-50, 456420}, NTCTableValue{-49, 427721}, NTCTableValue{-48, 401018},
        NTCTableValue{-47, 376161}, NTCTableValue{-46, 353009}, NTCTableValue{-45, 331435}, NTCTableValue{-44, 311323},
        NTCTableValue{-43, 292563}, NTCTableValue{-42, 275057}, NTCTableValue{-41, 258714}, NTCTableValue{-40, 243448},
        NTCTableValue{-39, 229182}, NTCTableValue{-38, 215846}, NTCTableValue{-37, 203371}, NTCTableValue{-36, 191699},
        NTCTableValue{-35, 180771}, NTCTableValue{-34, 170537}, NTCTableValue{-33, 160948}, NTCTableValue{-32, 151960},
        NTCTableValue{-31, 143530}, NTCTableValue{-30, 135622}, NTCTableValue{-29, 128200}, NTCTableValue{-28, 121231},
        NTCTableValue{-27, 114685}, NTCTableValue{-26, 108534}, NTCTableValue{-25, 102751}, NTCTableValue{-24, 97313},
        NTCTableValue{-23, 92196}, NTCTableValue{-22, 87380}, NTCTableValue{-21, 82846}, NTCTableValue{-20, 78575},
        NTCTableValue{-19, 74551}, NTCTableValue{-18, 70757}, NTCTableValue{-17, 67180}, NTCTableValue{-16, 63806},
        NTCTableValue{-15, 60622}, NTCTableValue{-14, 57616}, NTCTableValue{-13, 54778}, NTCTableValue{-12, 52096},
        NTCTableValue{-11, 49563}, NTCTableValue{-10, 47167}, NTCTableValue{-9, 44902}, NTCTableValue{-8, 42759},
        NTCTableValue{-7, 40732}, NTCTableValue{-6, 38812}, NTCTableValue{-5, 36995}, NTCTableValue{-4, 35273},
        NTCTableValue{-3, 33642}, NTCTableValue{-2, 32096}, NTCTableValue{-1, 30630}, NTCTableValue{0, 29239},
        NTCTableValue{1, 27920}, NTCTableValue{2, 26668}, NTCTableValue{3, 25480}, NTCTableValue{4, 24351},
        NTCTableValue{5, 23279}, NTCTableValue{6, 22261}, NTCTableValue{7, 21293}, NTCTableValue{8, 20372},
        NTCTableValue{9, 19497}, NTCTableValue{10, 18664}, NTCTableValue{11, 17872}, NTCTableValue{12, 17117},
        NTCTableValue{13, 16399}, NTCTableValue{14, 15715}, NTCTableValue{15, 15064}, NTCTableValue{16, 14443},
        NTCTableValue{17, 13851}, NTCTableValue{18, 13287}, NTCTableValue{19, 12749}, NTCTableValue{20, 12236},
        NTCTableValue{21, 11746}, NTCTableValue{22, 11279}, NTCTableValue{23, 10833}, NTCTableValue{24, 10406},
        NTCTableValue{25, 10000}, NTCTableValue{26, 9611}, NTCTableValue{27, 9239}, NTCTableValue{28, 8884},
        NTCTableValue{29, 8544}, NTCTableValue{30, 8220}, NTCTableValue{31, 7909}, NTCTableValue{32, 7612},
        NTCTableValue{33, 7328}, NTCTableValue{34, 7055}, NTCTableValue{35, 6795}, NTCTableValue{36, 6545},
        NTCTableValue{37, 6306}, NTCTableValue{38, 6077}, NTCTableValue{39, 5857}, NTCTableValue{40, 5647},
        NTCTableValue{41, 5445}, NTCTableValue{42, 5252}, NTCTableValue{43, 5066}, NTCTableValue{44, 4888},
        NTCTableValue{45, 4717}, NTCTableValue{46, 4553}, NTCTableValue{47, 4396}, NTCTableValue{48, 4244},
        NTCTableValue{49, 4099}, NTCTableValue{50, 3960}, NTCTableValue{51, 3826}, NTCTableValue{52, 3697},
        NTCTableValue{53, 3573}, NTCTableValue{54, 3454}, NTCTableValue{55, 3340}, NTCTableValue{56, 3230},
        NTCTableValue{57, 3124}, NTCTableValue{58, 3022}, NTCTableValue{59, 2924}, NTCTableValue{60, 2830},
        NTCTableValue{61, 2739}, NTCTableValue{62, 2652}, NTCTableValue{63, 2567}, NTCTableValue{64, 2486},
        NTCTableValue{65, 2408}, NTCTableValue{66, 2333}, NTCTableValue{67, 2260}, NTCTableValue{68, 2190},
        NTCTableValue{69, 2123}, NTCTableValue{70, 2058}, NTCTableValue{71, 1995}, NTCTableValue{72, 1935},
        NTCTableValue{73, 1876}, NTCTableValue{74, 1820}, NTCTableValue{75, 1766}, NTCTableValue{76, 1713},
        NTCTableValue{77, 1663}, NTCTableValue{78, 1614}, NTCTableValue{79, 1567}, NTCTableValue{80, 1521},
        NTCTableValue{81, 1477}, NTCTableValue{82, 1434}, NTCTableValue{83, 1393}, NTCTableValue{84, 1353},
        NTCTableValue{85, 1315}, NTCTableValue{86, 1278}, NTCTableValue{87, 1242}, NTCTableValue{88, 1207},
        NTCTableValue{89, 1173}, NTCTableValue{90, 1141}, NTCTableValue{91, 1109}, NTCTableValue{92, 1079},
        NTCTableValue{93, 1050}, NTCTableValue{94, 1021}, NTCTableValue{95, 993}, NTCTableValue{96, 967},
        NTCTableValue{97, 941}, NTCTableValue{98, 916}, NTCTableValue{99, 891}, NTCTableValue{100, 868},
        NTCTableValue{101, 845}, NTCTableValue{102, 823}, NTCTableValue{103, 802}, NTCTableValue{104, 781},
        NTCTableValue{105, 761}, NTCTableValue{106, 741}, NTCTableValue{107, 722}, NTCTableValue{108, 704},
        NTCTableValue{109, 686}, NTCTableValue{110, 669}, NTCTableValue{111, 652}, NTCTableValue{112, 636},
        NTCTableValue{113, 620}, NTCTableValue{114, 605}, NTCTableValue{115, 590}, NTCTableValue{116, 575},
        NTCTableValue{117, 561}, NTCTableValue{118, 548}, NTCTableValue{119, 534}, NTCTableValue{120, 521},
        NTCTableValue{121, 509}, NTCTableValue{122, 497}, NTCTableValue{123, 485}, NTCTableValue{124, 474},
        NTCTableValue{125, 462}, NTCTableValue{126, 452}, NTCTableValue{127, 441}, NTCTableValue{128, 431},
        NTCTableValue{129, 421}, NTCTableValue{130, 411}, NTCTableValue{131, 402}, NTCTableValue{132, 393},
        NTCTableValue{133, 384}, NTCTableValue{134, 375}, NTCTableValue{135, 367}, NTCTableValue{136, 358},
        NTCTableValue{137, 350}, NTCTableValue{138, 343}, NTCTableValue{139, 335}, NTCTableValue{140, 328},
        NTCTableValue{141, 320}, NTCTableValue{142, 313}, NTCTableValue{143, 307}, NTCTableValue{144, 300},
        NTCTableValue{145, 294}, NTCTableValue{146, 287}, NTCTableValue{147, 281}, NTCTableValue{148, 275},
        NTCTableValue{149, 269}, NTCTableValue{150, 264});

  Drv::Sensor::Status NTC::getPolynomTemperature_handler(NATIVE_INT_TYPE portNum, F32& temperature) {
    U32 adcCode = 0;
    Drv::ADC::Status ADCCodeRetrieveStatus    = getADCCode_out(0, adcCode);
    if (ADCCodeRetrieveStatus != Drv::ADC::Status::SUCCESS) {
      return Drv::Sensor::Status::FAILED;
    }

    F32 ntcResistance = static_cast<F32>(calculateNTCResistanceBasedOnADC(adcCode));
    auto calculatedTemperature = getPolynomialTemperature(ntcResistance);
    
    temperature = calibrCoeffK * calculatedTemperature + calibrCoeffB;
    
    return Drv::Sensor::Status::SUCCESS;
  }

  Drv::Sensor::Status NTC::getTableTemperature_handler(NATIVE_INT_TYPE portNum, F32& temperature) {
    auto temperatureResult = getTemperature();
    if (temperatureResult.has_value() == false) {
      return Drv::Sensor::Status::FAILED;
    }

    temperature = calibrCoeffK * (temperatureResult.value()) + calibrCoeffB;

    return Drv::Sensor::Status::SUCCESS;
  }

etl::expected<F32, NTC::ErrorCode> NTC::getTemperature() {
    F32 voltageValue = 0.0;
    Drv::ADC::Status voltageMeasurementStatus = getVoltageValue_out(0, voltageValue);
    if (voltageMeasurementStatus != Drv::ADC::Status::SUCCESS) {
      return etl::unexpected(NTC::ErrorCode::VoltageMeasurementError);
    }
    F32 ntcVoltage = voltageValue;

    U32 ntcResistance = calculateNTCResistance(ntcVoltage);

    if (isNTCResistanceInRange(ntcResistance)) {
        closestValues resistancePair = searchClosestResistancesInTable(ntcResistance);
        return getInterpolatedTemperature(resistancePair, ntcResistance);
    }
    return etl::unexpected(NTC::ErrorCode::OutOfRangeError);
}

F32 NTC::calculateNTCResistance(F32 ntcVoltage) {
  constexpr F32 acceptableVoltageDiff = 0.01F;
  F32 voltageReference = this->getVoltageReference_out(0);

  const F32 voltageDiff = voltageReference - ntcVoltage;
  if (voltageDiff < acceptableVoltageDiff) {
      return UINT32_MAX;
  }
  return ntcVoltage * static_cast<F32>(upperArmResistance) / (voltageDiff);
}

F32 NTC::calculateNTCResistanceBasedOnADC(U32 adcCode) {
  U32 maxADCCode = this->getADCMax_out(0);
  
  F32 denominator = static_cast<float>(maxADCCode)/static_cast<float>(adcCode) - 1.0F;
  if (0 == denominator) {
    return UINT32_MAX;
  }

  return static_cast<F32>(upperArmResistance) / denominator;
}

bool NTC::isNTCResistanceInRange(U32 ntcResistance) const {
    return ((ntcResistance <= ntcTable[0].resistance) && (ntcResistance >= ntcTable[ntcTable.size() - 1].resistance));
}

NTC::closestValues NTC::searchClosestResistancesInTable(U32 ntcResistance) const {
    closestValues pair;
    int16_t mid = 0;
    pair.first = 0;
    pair.second = ntcTable.size();

    while ((pair.second - pair.first) > 1) {
        mid = static_cast<int16_t>((pair.first + pair.second) / 2);
        if (ntcResistance < ntcTable[mid].resistance) {
            pair.first = mid;
        } else {
            pair.second = mid;
        }
    }

    return pair;
}

F32 NTC::getInterpolatedTemperature(closestValues &pair, U32 ntcResistance) const {
  auto resultTemperature =
            static_cast<F32>((ntcTable[pair.first].resistance - ntcResistance)) * static_cast<F32>(ntcTable[pair.second].temperature);
    resultTemperature +=
            static_cast<F32>((ntcResistance - ntcTable[pair.second].resistance)) * static_cast<F32>(ntcTable[pair.first].temperature);
    resultTemperature =
            resultTemperature / static_cast<F32>(ntcTable[pair.first].resistance - ntcTable[pair.second].resistance);
    return resultTemperature;
}

F32 NTC::getPolynomialTemperature(F32 ntcResistance) const {
  auto lnNtcResistance = log(ntcResistance);
  auto ntcResistanceLog3 = lnNtcResistance * lnNtcResistance * lnNtcResistance;
  auto ntcResistanceLog2 = lnNtcResistance * lnNtcResistance;
  auto ntcResistanceLog1 = lnNtcResistance;

  F32 temperature = coeffX3 * ntcResistanceLog3 + coeffX2 * ntcResistanceLog2 + coeffX * ntcResistanceLog1 + coeffB;

  return temperature;
}

}
