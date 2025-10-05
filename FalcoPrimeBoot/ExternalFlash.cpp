#include "ExternalFlash.hpp"

bool exitQPIMode() {
    qspi_command_xfer_t nativeConfig{};
    nativeConfig.addr_en = false;
    nativeConfig.instruction = exitQPIModeCode;
    nativeConfig.addr_len = ADDRL_32_BIT;
    nativeConfig.width = QUAD_CMD;

    bool result = QSPI_CommandWrite(&nativeConfig, 0);

    return result;
}

bool getStatusRegister1(uint32_t* statusRegister1, QSPI_LANE_WIDTH qspiLaneWidth) {
    qspi_register_xfer_t nativeConfig{};
    nativeConfig.instruction = readStatusRG1;
    nativeConfig.width = qspiLaneWidth;
    nativeConfig.dummy_cycles = 0;
    uint32_t status = 0;
    const uint32_t sizeOfDataToRetrieve = 1;
    bool result = QSPI_RegisterRead(&nativeConfig, &status, sizeOfDataToRetrieve);
    *statusRegister1 = status;
    return result;
}

bool getStatusRegister2(uint32_t* statusRegister2, QSPI_LANE_WIDTH qspiLaneWidth) {
    qspi_register_xfer_t nativeConfig{};
    nativeConfig.instruction = readStatusRG2;
    nativeConfig.width = qspiLaneWidth;
    nativeConfig.dummy_cycles = 0;
    uint32_t status = 0;
    const uint32_t sizeOfDataToRetrieve = 1;
    bool result = QSPI_RegisterRead(&nativeConfig, &status, sizeOfDataToRetrieve);
    *statusRegister2 = status;
    return result;
}

bool writeRegister2(uint32_t value, QSPI_LANE_WIDTH qspiLaneWidth) {
    qspi_register_xfer_t nativeConfig;
    nativeConfig.instruction = writeStatusRG2;
    nativeConfig.width = qspiLaneWidth;
    nativeConfig.dummy_cycles = 0;

    bool result = QSPI_RegisterWrite(&nativeConfig, &value, 1);
    if (result == false) {
        return result;
    }

    result = waitUntilFlashReady(qspiLaneWidth);
    if (result == false) {
        return result;
    }
    
    return result;
}

bool writeEnableVolatile(QSPI_LANE_WIDTH qspiLaneWidth) { 
    qspi_command_xfer_t nativeConfig{};
    nativeConfig.addr_en = false;
    nativeConfig.instruction = volatileWriteEnable;
    nativeConfig.addr_len = ADDRL_32_BIT;
    nativeConfig.width = qspiLaneWidth;

    bool result = QSPI_CommandWrite(&nativeConfig, 0);
    
    return result;
} 

bool quadEnable(QSPI_LANE_WIDTH qspiLaneWidth) {
    uint32_t statusRegister2 = 0;
    bool result = getStatusRegister2(&statusRegister2, qspiLaneWidth);
    if (result == false) {
        return result;
    }

    result = writeEnableVolatile(qspiLaneWidth);
    if (result == false) {
        return result;
    }

    uint8_t quadEnableVar = statusRegister2;
    const uint8_t quadEnableMask = 0b1111'1101;
    quadEnableVar &= quadEnableMask;
    quadEnableVar |= static_cast<uint8_t>(~quadEnableMask);

    // writeRegister
    result = writeRegister2(quadEnableVar, qspiLaneWidth);
    if (result == false) {
        return result;
    }

    return true;
}

bool enterQPIMode() {
    qspi_command_xfer_t nativeConfig;
    nativeConfig.addr_en = false;
    nativeConfig.instruction = enterQPIModeCode;
    nativeConfig.addr_len = ADDRL_32_BIT;
    nativeConfig.width = SINGLE_BIT_SPI;

    bool result = QSPI_CommandWrite(&nativeConfig, 0);

    return result;
}

bool selectDie(uint8_t die) {
    qspi_register_xfer_t nativeConfig;
    nativeConfig.instruction    = softwareDieSelect;
    nativeConfig.width          = SINGLE_BIT_SPI;
    nativeConfig.dummy_cycles   = 0;
    uint32_t selectedDie = die;
    bool result = QSPI_RegisterWrite(&nativeConfig, &selectedDie, 1);

    result = waitUntilFlashReady(SINGLE_BIT_SPI);
    if (result == false) {
        return result;
    }

    return result;
}

bool waitUntilFlashReady(QSPI_LANE_WIDTH qspiLaneWidth) {
    bool exit = false;
    bool result = false;

    while (!exit) {
        uint32_t statusRegister1;
        result = getStatusRegister1(&statusRegister1, qspiLaneWidth);
        if (result == true) {
            const uint8_t StatusRegister1BUSYBitMask = 1U;
            if ((statusRegister1 & StatusRegister1BUSYBitMask) == 0) {
                exit = true;
            }
        } else {
            exit = true;
        }
    }

    return result;
}

bool reset() {
    bool result = selectDie(DIE1);
    if (result == false) {
        return result;
    }

    result = selectDie(DIE0);
    if (result == false) {
        return result;
    }

    qspi_command_xfer_t nativeConfig{};
    nativeConfig.addr_en = false;
    nativeConfig.instruction = resetEnable;
    nativeConfig.addr_len = ADDRL_32_BIT;
    nativeConfig.width = SINGLE_BIT_SPI;

    result = QSPI_CommandWrite(&nativeConfig, 0);
    if (result == false) {
        return result;
    }

    nativeConfig.instruction = resetDevice;

    result = QSPI_CommandWrite(&nativeConfig, 0);
    if (result == false) {
        return result;
    }

    return result;
}

bool readID(uint32_t *id) {
    qspi_register_xfer_t nativeConfig;
    nativeConfig.instruction = readJedecIdCode;
    nativeConfig.width = SINGLE_BIT_SPI;
    nativeConfig.dummy_cycles = 0;

    uint8_t deviceID[3] = {0};
    bool result = QSPI_RegisterRead(&nativeConfig, (uint32_t *)(deviceID), sizeof(deviceID) / sizeof(deviceID[0]));
    if (result == false) {
        return false;
    }

    const uint8_t mask8BitShift = 8U;
    uint32_t receivedID = 0;
    for (size_t i = 0; i < sizeof(deviceID) / sizeof(deviceID[0]); i++) {
        receivedID |= (deviceID[i] << (mask8BitShift * i));
    }
    
    *id = receivedID;
    
    return true;
}

bool enter4ByteAddressMode() {
    qspi_command_xfer_t nativeConfig{};
    nativeConfig.addr_en = false;
    nativeConfig.instruction = enter4ByteAddressModeCode;
    nativeConfig.addr_len = ADDRL_32_BIT;
    nativeConfig.width = SINGLE_BIT_SPI;

    bool result = QSPI_CommandWrite(&nativeConfig, 0);

    return result;
}

bool setReadParameters() {
    qspi_register_xfer_t nativeConfig{};
    nativeConfig.instruction    = setReadParametersCode;
    nativeConfig.width          = SINGLE_BIT_SPI;
    nativeConfig.dummy_cycles   = 0;

    const uint8_t numOfDummyClocks8 = 0b0011'0000;
    bool result = QSPI_RegisterWrite(&nativeConfig, (uint32_t *)(numOfDummyClocks8), sizeof(numOfDummyClocks8));
    if (result == false) {
        return result;
    }

    result = waitUntilFlashReady(SINGLE_BIT_SPI);
    if (result == false) {
        return result;
    }

    return result;
}



bool enableContinuousRead() {
    qspi_memory_xfer_t nativeConfig;
    nativeConfig.instruction = fastReadQuadIO4byteCode;
    const uint8_t CONTINUOUS_READ_OPTION = 0b0010'0000; 
    nativeConfig.option = CONTINUOUS_READ_OPTION;
    nativeConfig.width = QUAD_IO;
    nativeConfig.addr_len = ADDRL_32_BIT;
    nativeConfig.option_en = true;
    nativeConfig.option_len = OPTL_8_BIT;
    nativeConfig.continuous_read_en = true; // continuous read enable
    nativeConfig.dummy_cycles = 4;

    uint32_t data = 0;
    uint32_t memoryAddress = 0;

    bool result = QSPI_MemoryRead(&nativeConfig, &data, 4, memoryAddress);
    if (result == false) {
        return false;
    }

    return true;
}

bool externalFlashInit() {
    bool result = exitQPIMode();
    if (result == false) {
        return result;
    }

    result = reset();
    if (result == false) {
        return result;
    }

    uint32_t jedecID = 0;
    result = readID(&jedecID);
    if (result == false) {
        return result;
    }

    if (W25Q01JV_ID != jedecID) {
        result = false;
        return false;
    }

    result = enter4ByteAddressMode();
    if (result == false) {
        return result;
    }

    return true;
}
