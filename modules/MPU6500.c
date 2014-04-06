#include "MPU6500.h"
#include "i2c.h"


unsigned char MPU6500_Init()
{
        unsigned char devAddr = MPU6500_DEFAULT_ADDRESS;
	// setClockSource
	writeBits(devAddr, MPU6500_RA_PWR_MGMT_1, MPU6500_PWR1_CLKSEL_BIT, MPU6500_PWR1_CLKSEL_LENGTH, MPU6500_CLOCK_PLL_XGYRO);
	// setFullScaleGyroRange
	writeBits(devAddr, MPU6500_RA_GYRO_CONFIG, MPU6500_GCONFIG_FS_SEL_BIT, MPU6500_GCONFIG_FS_SEL_LENGTH, MPU6500_GYRO_FS_250);
	// setFullScaleAccelRange
	writeBits(devAddr, MPU6500_RA_ACCEL_CONFIG, MPU6500_ACONFIG_AFS_SEL_BIT, MPU6500_ACONFIG_AFS_SEL_LENGTH, MPU6500_ACCEL_FS_2);
	// setSleepEnabled
	writeBit(devAddr, MPU6500_RA_PWR_MGMT_1, MPU6500_PWR1_SLEEP_BIT, 0);
	// Enable DMP
	writeBit(devAddr, MPU6500_RA_USER_CTRL, MPU6500_USERCTRL_DMP_EN_BIT, 1);
        
        return 0;
}

unsigned char Test_Connection()
{
  return 0;
}