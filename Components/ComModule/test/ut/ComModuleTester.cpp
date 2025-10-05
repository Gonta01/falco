// ======================================================================
// \title  ComModuleTester.cpp
// \author aos
// \brief  cpp file for ComModule component test harness implementation class
// ======================================================================

#include "ComModuleTester.hpp"

namespace Components
{

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  ComModuleTester ::
      ComModuleTester() : ComModuleGTestBase("ComModuleTester", ComModuleTester::MAX_HISTORY_SIZE),
                          component("ComModule")
  {
    this->initComponents();
    this->connectPorts();
  }

  ComModuleTester ::
      ~ComModuleTester()
  {
  }

  // ----------------------------------------------------------------------
  // Tests
  // ----------------------------------------------------------------------

  void ComModuleTester ::
      testComModuleRoutine()
  {
    this->component.configure();

    this->setMockWriteStatus(Drv::UART::WriteStatus::SUCCESS);
    etl::vector<U8, 64> dataToSend = {0x01, 0x06, 0x0B, 0x7C};
    Fw::Buffer sendBuffer(dataToSend.data(), dataToSend.size());
    auto status = this->invoke_to_send(0, sendBuffer);
    ASSERT_EQ(status, Drv::SendStatus::SEND_OK);

    etl::vector<U8, 64> readData = {0x04, 0x01, 0xBA, 0x0F};
    this->setMockReadBuffer(readData);
    this->invoke_to_schedIn(0, 0);
    ASSERT_EQ(this->getCapturedRecvStatus(), Drv::RecvStatus::RECV_OK);
  }

  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------

  Fw::Buffer ComModuleTester ::
      from_allocate_handler(
          NATIVE_INT_TYPE portNum,
          U32 size)
  {
    etl::vector<U8, 64> data;
    Fw::Buffer allocatedBuffer(data.data(), data.size());
    return allocatedBuffer;
  }

  void ComModuleTester ::
      from_deallocate_handler(
          NATIVE_INT_TYPE portNum,
          Fw::Buffer &fwBuffer)
  {
    fwBuffer.setData(nullptr);
    fwBuffer.setSize(0);
  }

  U32 ComModuleTester ::
      from_read_handler(
          NATIVE_INT_TYPE portNum,
          Fw::Buffer &buffer,
          U32 timeoutMs)
  {
    U8 *data = buffer.getData();
    U8 sizeOfData = mockedReadBuffers.size();
    for (size_t i = 0; i < sizeOfData; i++)
    {
      data[i] = mockedReadBuffers[i];
    }
    return sizeOfData;
  }

  void ComModuleTester ::
      from_ready_handler(NATIVE_INT_TYPE portNum)
  {
    // TODO
  }

  void ComModuleTester ::
      from_recv_handler(
          NATIVE_INT_TYPE portNum,
          Fw::Buffer &recvBuffer,
          const Drv::RecvStatus &recvStatus)
  {
    capturedRecvStatus = recvStatus;
  }

  Drv::UART::WriteStatus ComModuleTester ::
      from_write_handler(
          NATIVE_INT_TYPE portNum,
          Fw::Buffer &buffer,
          U32 timeoutMs)
  {
    return mockedWriteStatus;
  }

  void ComModuleTester ::
      setMockWriteStatus(Drv::UART::WriteStatus status)
  {
    mockedWriteStatus = status;
  }

  void ComModuleTester ::
      setMockReadBuffer(etl::vector<U8, 64> value)
  {
    mockedReadBuffers.clear();
    for (size_t i = 0; i < 64; i++)
    {
      mockedReadBuffers[i] = value[i];
    }
  }

  Drv::RecvStatus & ComModuleTester :: getCapturedRecvStatus()
  {
    return this->capturedRecvStatus;
  }
}
