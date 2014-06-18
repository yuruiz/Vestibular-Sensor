#include "MPU6500.h"
#include "i2c.h"
#include "config.h"

unsigned char MPU6500_Init()
{
    unsigned char devAddr = MPU6500_DEFAULT_ADDRESS;
    // setClockSource
    writeBits(devAddr, MPU6500_RA_PWR_MGMT_1, MPU6500_PWR1_CLKSEL_BIT, MPU6500_PWR1_CLKSEL_LENGTH,
    		  MPU6500_CLOCK_PLL_XGYRO);
    // setFullScaleGyroRange
    writeBits(devAddr, MPU6500_RA_GYRO_CONFIG, MPU6500_GCONFIG_FS_SEL_BIT, MPU6500_GCONFIG_FS_SEL_LENGTH,
              MPU6500_GYRO_FS_1000);
    // setFullScaleAccelRange
    writeBits(devAddr, MPU6500_RA_ACCEL_CONFIG, MPU6500_ACONFIG_AFS_SEL_BIT, MPU6500_ACONFIG_AFS_SEL_LENGTH,
              MPU6500_ACCEL_FS_8);
    // setSleepEnabled
    writeBit(devAddr, MPU6500_RA_PWR_MGMT_1, MPU6500_PWR1_SLEEP_BIT, 0);
    // Enable DMP
    writeBit(devAddr, MPU6500_RA_USER_CTRL, MPU6500_USERCTRL_DMP_EN_BIT, 1);

#ifdef FIFO
    FIFO_Enable();
#endif

    return 0;
}

unsigned char MPU_Test_Connection()
{
	unsigned char ID = ReadByte(MPU6500_DEFAULT_ADDRESS, MPU6500_RA_WHO_AM_I);

	if (ID == 0x70)
	{
		return 1;
	}
    return 0;
}

unsigned char FIFO_Enable()
{
    unsigned char devAddr = MPU6500_DEFAULT_ADDRESS;

    writeBit(devAddr, MPU6500_RA_FIFO_EN, MPU6500_XG_FIFO_EN_BIT, 1);
    writeBit(devAddr, MPU6500_RA_FIFO_EN, MPU6500_YG_FIFO_EN_BIT, 1);
    writeBit(devAddr, MPU6500_RA_FIFO_EN, MPU6500_ZG_FIFO_EN_BIT, 1);
    writeBit(devAddr, MPU6500_RA_FIFO_EN, MPU6500_ACCEL_FIFO_EN_BIT, 1);

    return 1;
}
