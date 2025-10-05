// ======================================================================
// \title  BNO055TestMain.cpp
// \author aos
// \brief  cpp file for BNO055 component test main function
// ======================================================================

#include "BNO055Tester.hpp"

TEST(TestReadBNO055, testReadData)
{
  Components::BNO055Tester tester;
  tester.testReadBNO055();
}

TEST(TestWriteBNO055, testWriteData)
{
  Components::BNO055Tester tester;
  tester.testWriteBNO055();
}

TEST(TestGetAccelBNO055, testGetAccelData)
{
  Components::BNO055Tester tester;
  tester.testGetAccelBNO055();
}

TEST(TestGetGyroBNO055, testGetGyroData)
{
  Components::BNO055Tester tester;
  tester.testGetGyroBNO055();
}

TEST(TestGetMagnBNO055, testGetMagnData)
{
  Components::BNO055Tester tester;
  tester.testGetMagnBNO055();
}

TEST(TestGetRawBNO055, testGetRawData)
{
  Components::BNO055Tester tester;
  tester.testGetRawBNO055();
}

TEST(TestGetQuatBNO055, testGetQuatData)
{
  Components::BNO055Tester tester;
  tester.testGetQuatBNO055();
}

TEST(TestConfigureBNO055, testConfigure)
{
  Components::BNO055Tester tester;
  tester.testConfigureBNO055();
}

TEST(TestFailureWriteFailBNO055, testFailureComm)
{
  Components::BNO055Tester tester;
  tester.testFailureWriteFailBNO055();
}

TEST(TestFailureReadFailBNO055, testFailureComm)
{
  Components::BNO055Tester tester;
  tester.testFailureReadFailBNO055();
}

TEST(testFailureRegmapInvalidAddressBNO055, testFailureComm)
{
  Components::BNO055Tester tester;
  tester.testFailureRegmapInvalidAddressBNO055();
}

TEST(TestFailureRegmapWriteDisabledBNO055, testFailureComm)
{
  Components::BNO055Tester tester;
  tester.testFailureRegmapWriteDisabledBNO055();
}

TEST(TestFailureWrongStartByteBNO055, testFailureComm)
{
  Components::BNO055Tester tester;
  tester.testFailureWrongStartByteBNO055();
}

TEST(TestFailureMaxLengthErrorBNO055, testFailureComm)
{
  Components::BNO055Tester tester;
  tester.testFailureMaxLengthErrorBNO055();
}

TEST(TestFailureMinLengthErrorBNO055, testFailureComm)
{
  Components::BNO055Tester tester;
  tester.testFailureMinLengthErrorBNO055();
}

TEST(TestFailureRecieveCharacterTimeoutBNO05, testFailureComm)
{
  Components::BNO055Tester tester;
  tester.testFailureRecieveCharacterTimeoutBNO055();
}

TEST(TestFailureResponseHeaderMismatchBNO055, testFailure)
{
  Components::BNO055Tester tester;
  tester.testFailureResponseHeaderMismatchBNO055();
}
TEST(TestFailureResponseSizeMismatchBNO055, testFailure)
{
  Components::BNO055Tester tester;
  tester.testFailureResponseSizeMismatchBNO055();
}

TEST(TestFailureReceiveBNO055, testFailure)
{
  Components::BNO055Tester tester;
  tester.testFailureReceiveBNO055();
}

TEST(TestFailureSendBNO055, testFailure)
{
  Components::BNO055Tester tester;
  tester.testFailureSendBNO055();
}

TEST(TestFailureInitBNO055, testFailure)
{
  Components::BNO055Tester tester;
  tester.testFailureInitBNO055();
}

TEST(TestFailureSelfTestBNO055, testFailure)
{
  Components::BNO055Tester tester;
  tester.testFailureSelfTestBNO055();
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
