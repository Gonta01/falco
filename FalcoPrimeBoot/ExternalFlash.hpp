#include <cstdbool>
#include "definitions.h"

#ifndef EXTERNAL_FLASH_H
#define EXTERNAL_FLASH_H

#define enterQPIModeCode            (0x38)
#define exitQPIModeCode             (0xFF)
#define readJedecIdCode             (0x9F)
#define enter4ByteAddressModeCode   (0xB7)
#define setReadParametersCode       (0xC0)
#define fastReadQuadIO4byteCode     (0xEC)

#define readStatusRG1               (0x05)
#define readStatusRG2               (0x35)
#define writeStatusRG2              (0x31)
#define volatileWriteEnable         (0x50)
#define softwareDieSelect           (0xC2)

#define resetDevice                 (0x99)
#define resetEnable                 (0x66)
#define DIE0                        (0)
#define DIE1                        (1)

#define W25Q01JV_ID                 (0x2170EFUL)

bool externalFlashInit();
bool enableContinuousRead();

bool exitQPIMode();
bool getStatusRegister1(uint32_t* statusRegister1, QSPI_LANE_WIDTH qspiLaneWidth);
bool getStatusRegister2(uint32_t* statusRegister2, QSPI_LANE_WIDTH qspiLaneWidth);
bool writeRegister2(uint32_t value, QSPI_LANE_WIDTH qspiLaneWidth);
bool writeEnableVolatile(QSPI_LANE_WIDTH qspiLaneWidth);
bool quadEnable(QSPI_LANE_WIDTH qspiLaneWidth);
bool enterQPIMode();
bool selectDie(uint8_t die);
bool waitUntilFlashReady(QSPI_LANE_WIDTH qspiLaneWidth);
bool reset();
bool readID(uint32_t *id);
bool enter4ByteAddressMode();
bool setReadParameters();
bool enableContinuousRead();
bool externalFlashInit();


#endif // EXTERNAL_FLASH_H
