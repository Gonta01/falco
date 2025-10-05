// ======================================================================
// \title  ComModuleTestMain.cpp
// \author aos
// \brief  cpp file for ComModule component test main function
// ======================================================================

#include "ComModuleTester.hpp"

TEST(TestComModuleRoutine, toDo) {
  Components::ComModuleTester tester;
  tester.testComModuleRoutine();
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
