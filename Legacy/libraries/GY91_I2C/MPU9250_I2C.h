/*
 $License:
    Copyright (C) 2011-2012 InvenSense Corporation, All Rights Reserved.
    See included License.txt for License information.
 $
 */
/**
 *  @addtogroup  DRIVERS Sensor Driver Layer
 *  @brief       Hardware drivers to communicate with sensors via I2C.
 *
 *  @{
 *      @file       inv_mpu.h
 *      @brief      An I2C-based driver for Invensense gyroscopes.
 *      @details    This driver currently works for the following devices:
 *                  MPU6500
 *                  MPU9250 (or MPU6500 w/ AK8963 on the auxiliary bus)
 */

#ifndef _MPU9250_I2C_H_
#define _MPU9250_I2C_H_

#include "I2Cdev_GY91.h"
#define MPU9250 // select your chip

//MPU9250 Register map

#define MPU9250_DEFAULT_ADDRESS         0x68
#define MPU9250_ALT_DEFAULT_ADDRESS     0x69   

#define MPU9250_SELF_TEST_X_GYRO        0x00
#define MPU9250_SELF_TEST_Y_GYRO        0x01
#define MPU9250_SELF_TEST_Z_GYRO        0x02

#define MPU9250_SELF_TEST_X_ACCEL       0x0D
#define MPU9250_SELF_TEST_Y_ACCEL       0x0E
#define MPU9250_SELF_TEST_Z_ACCEL       0x0F

#define MPU9250_XG_OFFSET_H             0x13
#define MPU9250_XG_OFFSET_L             0x14
#define MPU9250_YG_OFFSET_H             0x15
#define MPU9250_YG_OFFSET_L             0x16
#define MPU9250_ZG_OFFSET_H             0x17
#define MPU9250_ZG_OFFSET_L             0x18
#define MPU9250_SMPLRT_DIV              0x19
#define MPU9250_CONFIG                  0x1A
#define MPU9250_GYRO_CONFIG             0x1B
#define MPU9250_ACCEL_CONFIG            0x1C
#define MPU9250_ACCEL_CONFIG2           0x1D
#define MPU9250_LP_ACCEL_ODR            0x1E
#define MPU9250_WOM_THR                 0x1F

#define MPU9250_FIFO_EN                 0x23
#define MPU9250_I2C_MST_CTRL            0x24
#define MPU9250_I2C_SLV0_ADDR           0x25
#define MPU9250_I2C_SLV0_REG            0x26
#define MPU9250_I2C_SLV0_CTRL           0x27
#define MPU9250_I2C_SLV1_ADDR           0x28
#define MPU9250_I2C_SLV1_REG            0x29
#define MPU9250_I2C_SLV1_CTRL           0x2A
#define MPU9250_I2C_SLV2_ADDR           0x2B
#define MPU9250_I2C_SLV2_REG            0x2C
#define MPU9250_I2C_SLV2_CTRL           0x2D
#define MPU9250_I2C_SLV3_ADDR           0x2E
#define MPU9250_I2C_SLV3_REG            0x2F
#define MPU9250_I2C_SLV3_CTRL           0x30
#define MPU9250_I2C_SLV4_ADDR           0x31
#define MPU9250_I2C_SLV4_REG            0x32
#define MPU9250_I2C_SLV4_DO             0x33
#define MPU9250_I2C_SLV4_CTRL           0x34
#define MPU9250_I2C_SLV4_DI             0x35
#define MPU9250_I2C_MST_STATUS          0x36
#define MPU9250_INT_PIN_CFG             0x37
#define MPU9250_INT_ENABLE              0x38

#define MPU9250_INT_STATUS              0x3A
#define MPU9250_ACCEL_XOUT_H            0x3B
#define MPU9250_ACCEL_XOUT_L            0x3C
#define MPU9250_ACCEL_YOUT_H            0x3D
#define MPU9250_ACCEL_YOUT_L            0x3E
#define MPU9250_ACCEL_ZOUT_H            0x3F
#define MPU9250_ACCEL_ZOUT_L            0x40
#define MPU9250_TEMP_OUT_H              0x41
#define MPU9250_TEMP_OUT_L              0x42
#define MPU9250_GYRO_XOUT_H             0x43
#define MPU9250_GYRO_XOUT_L             0x44
#define MPU9250_GYRO_YOUT_H             0x45
#define MPU9250_GYRO_YOUT_L             0x46
#define MPU9250_GYRO_ZOUT_H             0x47
#define MPU9250_GYRO_ZOUT_L             0x48
#define MPU9250_EXT_SENS_DATA_00        0x49
#define MPU9250_EXT_SENS_DATA_01        0x4A
#define MPU9250_EXT_SENS_DATA_02        0x4B
#define MPU9250_EXT_SENS_DATA_03        0x4C
#define MPU9250_EXT_SENS_DATA_04        0x4D
#define MPU9250_EXT_SENS_DATA_05        0x4E
#define MPU9250_EXT_SENS_DATA_06        0x4F
#define MPU9250_EXT_SENS_DATA_07        0x50
#define MPU9250_EXT_SENS_DATA_08        0x51
#define MPU9250_EXT_SENS_DATA_09        0x52
#define MPU9250_EXT_SENS_DATA_10        0x53
#define MPU9250_EXT_SENS_DATA_11        0x54
#define MPU9250_EXT_SENS_DATA_12        0x55
#define MPU9250_EXT_SENS_DATA_13        0x56
#define MPU9250_EXT_SENS_DATA_14        0x57
#define MPU9250_EXT_SENS_DATA_15        0x58
#define MPU9250_EXT_SENS_DATA_16        0x59
#define MPU9250_EXT_SENS_DATA_17        0x5A
#define MPU9250_EXT_SENS_DATA_18        0x5B
#define MPU9250_EXT_SENS_DATA_19        0x5C
#define MPU9250_EXT_SENS_DATA_20        0x5D
#define MPU9250_EXT_SENS_DATA_21        0x5E
#define MPU9250_EXT_SENS_DATA_22        0x5F
#define MPU9250_EXT_SENS_DATA_23        0x60

#define MPU9250_I2C_SLV0_DO             0x63
#define MPU9250_I2C_SLV1_DO             0x64
#define MPU9250_I2C_SLV2_DO             0x65
#define MPU9250_I2C_SLV3_DO             0x66
#define MPU9250_I2C_MST_DELAY_CTRL      0x67
#define MPU9250_SIGNAL_PATH_RESET       0x68
#define MPU9250_MOT_DETECT_CTRL         0x69
#define MPU9250_USER_CTRL               0x6A
#define MPU9250_PWR_MGMT_1              0x6B
#define MPU9250_PWR_MGMT_2              0x6C

#define MPU9250_FIFO_COUNTH             0x72
#define MPU9250_FIFO_COUNTL             0x73
#define MPU9250_FIFO_R_W                0x74
#define MPU9250_WHO_AM_I                0x75
#define MPU9250_XA_OFFSET_H             0x77
#define MPU9250_XA_OFFSET_L             0x78

#define MPU9250_YA_OFFSET_H             0x7A
#define MPU9250_YA_OFFSET_L             0x7B

#define MPU9250_ZA_OFFSET_H             0x7D
#define MPU9250_ZA_OFFSET_L             0x7E

//reset values
#define WHOAMI_RESET_VAL                0x71
#define POWER_MANAGMENT_1_RESET_VAL     0x01
#define DEFAULT_RESET_VALUE             0x00

#define WHOAMI_DEFAULT_VAL              0x68

//CONFIG register masks
#define MPU9250_FIFO_MODE_MASK          0x40
#define MPU9250_EXT_SYNC_SET_MASK       0x38
#define MPU9250_DLPF_CFG_MASK           0x07

//GYRO_CONFIG register masks
#define MPU9250_XGYRO_CTEN_MASK         0x80
#define MPU9250_YGYRO_CTEN_MASK         0x40
#define MPU9250_ZGYRO_CTEN_MASK         0x20
#define MPU9250_GYRO_FS_SEL_MASK        0x18
#define MPU9250_FCHOICE_B_MASK          0x03

#define MPU9250_GYRO_FULL_SCALE_250DPS  0
#define MPU9250_GYRO_FULL_SCALE_500DPS  1
#define MPU9250_GYRO_FULL_SCALE_1000DPS 2
#define MPU9250_GYRO_FULL_SCALE_2000DPS 3

//ACCEL_CONFIG register masks
#define MPU9250_AX_ST_EN_MASK           0x80
#define MPU9250_AY_ST_EN_MASK           0x40
#define MPU9250_AZ_ST_EN_MASK           0x20
#define MPU9250_ACCEL_FS_SEL_MASK       0x18

#define MPU9250_FULL_SCALE_2G           0
#define MPU9250_FULL_SCALE_4G           1
#define MPU9250_FULL_SCALE_8G           2
#define MPU9250_FULL_SCALE_16G          3

//ACCEL_CONFIG_2 register masks
#define MPU9250_ACCEL_FCHOICE_B_MASK    0xC0
#define MPU9250_A_DLPF_CFG_MASK         0x03

//LP_ACCEL_ODR register masks
#define MPU9250_LPOSC_CLKSEL_MASK       0x0F

//FIFO_EN register masks
#define MPU9250_TEMP_FIFO_EN_MASK       0x80
#define MPU9250_GYRO_XOUT_MASK          0x40
#define MPU9250_GYRO_YOUT_MASK          0x20
#define MPU9250_GYRO_ZOUT_MASK          0x10
#define MPU9250_ACCEL_MASK              0x08
#define MPU9250_SLV2_MASK               0x04
#define MPU9250_SLV1_MASK               0x02
#define MPU9250_SLV0_MASK               0x01

//I2C_MST_CTRL register masks
#define MPU9250_MULT_MST_EN_MASK        0x80
#define MPU9250_WAIT_FOR_ES_MASK        0x40
#define MPU9250_SLV_3_FIFO_EN_MASK      0x20
#define MPU9250_I2C_MST_P_NSR_MASK      0x10
#define MPU9250_I2C_MST_CLK_MASK        0x0F

//I2C_SLV0_ADDR register masks
#define MPU9250_I2C_SLV0_RNW_MASK       0x80
#define MPU9250_I2C_ID_0_MASK           0x7F

//I2C_SLV0_CTRL register masks
#define MPU9250_I2C_SLV0_EN_MASK        0x80
#define MPU9250_I2C_SLV0_BYTE_SW_MASK   0x40
#define MPU9250_I2C_SLV0_REG_DIS_MASK   0x20
#define MPU9250_I2C_SLV0_GRP_MASK       0x10
#define MPU9250_I2C_SLV0_LENG_MASK      0x0F

//I2C_SLV1_ADDR register masks
#define MPU9250_I2C_SLV1_RNW_MASK       0x80
#define MPU9250_I2C_ID_1_MASK           0x7F

//I2C_SLV1_CTRL register masks
#define MPU9250_I2C_SLV1_EN_MASK        0x80
#define MPU9250_I2C_SLV1_BYTE_SW_MASK   0x40
#define MPU9250_I2C_SLV1_REG_DIS_MASK   0x20
#define MPU9250_I2C_SLV1_GRP_MASK       0x10
#define MPU9250_I2C_SLV1_LENG_MASK      0x0F

//I2C_SLV2_ADDR register masks
#define MPU9250_I2C_SLV2_RNW_MASK       0x80
#define MPU9250_I2C_ID_2_MASK           0x7F

//I2C_SLV2_CTRL register masks
#define MPU9250_I2C_SLV2_EN_MASK        0x80
#define MPU9250_I2C_SLV2_BYTE_SW_MASK   0x40
#define MPU9250_I2C_SLV2_REG_DIS_MASK   0x20
#define MPU9250_I2C_SLV2_GRP_MASK       0x10
#define MPU9250_I2C_SLV2_LENG_MASK      0x0F

//I2C_SLV3_ADDR register masks
#define MPU9250_I2C_SLV3_RNW_MASK       0x80
#define MPU9250_I2C_ID_3_MASK           0x7F

//I2C_SLV3_CTRL register masks
#define MPU9250_I2C_SLV3_EN_MASK        0x80
#define MPU9250_I2C_SLV3_BYTE_SW_MASK   0x40
#define MPU9250_I2C_SLV3_REG_DIS_MASK   0x20
#define MPU9250_I2C_SLV3_GRP_MASK       0x10
#define MPU9250_I2C_SLV3_LENG_MASK      0x0F

//I2C_SLV4_ADDR register masks
#define MPU9250_I2C_SLV4_RNW_MASK       0x80
#define MPU9250_I2C_ID_4_MASK           0x7F

//I2C_SLV4_CTRL register masks
#define MPU9250_I2C_SLV4_EN_MASK        0x80
#define MPU9250_SLV4_DONE_INT_EN_MASK   0x40
#define MPU9250_I2C_SLV4_REG_DIS_MASK   0x20
#define MPU9250_I2C_MST_DLY_MASK        0x1F

//I2C_MST_STATUS register masks
#define MPU9250_PASS_THROUGH_MASK       0x80
#define MPU9250_I2C_SLV4_DONE_MASK      0x40
#define MPU9250_I2C_LOST_ARB_MASK       0x20
#define MPU9250_I2C_SLV4_NACK_MASK      0x10
#define MPU9250_I2C_SLV3_NACK_MASK      0x08
#define MPU9250_I2C_SLV2_NACK_MASK      0x04
#define MPU9250_I2C_SLV1_NACK_MASK      0x02
#define MPU9250_I2C_SLV0_NACK_MASK      0x01

//INT_PIN_CFG register masks
#define MPU9250_ACTL_MASK               0x80
#define MPU9250_OPEN_MASK               0x40
#define MPU9250_LATCH_INT_EN_MASK       0x20
#define MPU9250_INT_ANYRD_2CLEAR_MASK   0x10
#define MPU9250_ACTL_FSYNC_MASK         0x08
#define MPU9250_FSYNC_INT_MODE_EN_MASK  0x04
#define MPU9250_BYPASS_EN_MASK          0x02

//INT_ENABLE register masks
#define MPU9250_WOM_EN_MASK             0x40
#define MPU9250_FIFO_OFLOW_EN_MASK      0x10
#define MPU9250_FSYNC_INT_EN_MASK       0x08
#define MPU9250_RAW_RDY_EN_MASK         0x01

//INT_STATUS register masks
#define MPU9250_WOM_INT_MASK            0x40
#define MPU9250_FIFO_OFLOW_INT_MASK     0x10
#define MPU9250_FSYNC_INT_MASK          0x08
#define MPU9250_RAW_DATA_RDY_INT_MASK   0x01

//I2C_MST_DELAY_CTRL register masks
#define MPU9250_DELAY_ES_SHADOW_MASK    0x80
#define MPU9250_I2C_SLV4_DLY_EN_MASK    0x10
#define MPU9250_I2C_SLV3_DLY_EN_MASK    0x08
#define MPU9250_I2C_SLV2_DLY_EN_MASK    0x04
#define MPU9250_I2C_SLV1_DLY_EN_MASK    0x02
#define MPU9250_I2C_SLV0_DLY_EN_MASK    0x01

//SIGNAL_PATH_RESET register masks
#define MPU9250_GYRO_RST_MASK           0x04
#define MPU9250_ACCEL_RST_MASK          0x02
#define MPU9250_TEMP_RST_MASK           0x01

//MOT_DETECT_CTRL register masks
#define MPU9250_ACCEL_INTEL_EN_MASK     0x80
#define MPU9250_ACCEL_INTEL_MODE_MASK   0x40

//USER_CTRL register masks
#define MPU9250_FIFO_EN_MASK            0x40
#define MPU9250_I2C_MST_EN_MASK         0x20
#define MPU9250_I2C_IF_DIS_MASK         0x10
#define MPU9250_FIFO_RST_MASK           0x04
#define MPU9250_I2C_MST_RST_MASK        0x02
#define MPU9250_SIG_COND_RST_MASK       0x01

//PWR_MGMT_1 register masks
#define MPU9250_H_RESET_MASK            0x80
#define MPU9250_SLEEP_MASK              0x40
#define MPU9250_CYCLE_MASK              0x20
#define MPU9250_GYRO_STANDBY_CYCLE_MASK 0x10
#define MPU9250_PD_PTAT_MASK            0x08
#define MPU9250_CLKSEL_MASK             0x07

//PWR_MGMT_2 register masks
#define MPU9250_DISABLE_XA_MASK         0x20
#define MPU9250_DISABLE_YA_MASK         0x10
#define MPU9250_DISABLE_ZA_MASK         0x08
#define MPU9250_DISABLE_XG_MASK         0x04
#define MPU9250_DISABLE_YG_MASK         0x02
#define MPU9250_DISABLE_ZG_MASK         0x01

#define MPU9250_DISABLE_XYZA_MASK       0x38
#define MPU9250_DISABLE_XYZG_MASK       0x07

//Magnetometer register maps
#define MPU9250_MAG_ADDRESS             0x0C

#define MPU9250_MAG_WIA                 0x00
#define MPU9250_MAG_INFO                0x01
#define MPU9250_MAG_ST1                 0x02
#define MPU9250_MAG_XOUT_L              0x03
#define MPU9250_MAG_XOUT_H              0x04
#define MPU9250_MAG_YOUT_L              0x05
#define MPU9250_MAG_YOUT_H              0x06
#define MPU9250_MAG_ZOUT_L              0x07
#define MPU9250_MAG_ZOUT_H              0x08
#define MPU9250_MAG_ST2                 0x09
#define MPU9250_MAG_CNTL                0x0A
#define MPU9250_MAG_RSV                 0x0B //reserved mystery meat
#define MPU9250_MAG_ASTC                0x0C
#define MPU9250_MAG_TS1                 0x0D
#define MPU9250_MAG_TS2                 0x0E
#define MPU9250_MAG_I2CDIS              0x0F
#define MPU9250_MAG_ASAX                0x10
#define MPU9250_MAG_ASAY                0x11
#define MPU9250_MAG_ASAZ                0x12

//Magnetometer register masks
#define MPU9250_WIA_MASK 0x48

///VARIABLES STATICAS:
#define MPU9250_BANKSIZE                256
#define MPU9250_TEMPOFFSET              0
#define MPU9250_TEMPSENS                321
#define MPU9250_NUMREG                  128
#define MPU9250_MAXFIFO                 1024
#define MPU9250_RAWCOMPASS              0x49
#define MPU9250_YGOFFSTC                0x01
#define MPU9250_S1DO                    0x64
#define MPU9250_S0DO                    0x63
#define MPU9250_I2CDELAYCTRL            0x67
#define MPU9250_S4CTRL                  0x34
#define MPU9250_S1CTRL                  0x2A
#define MPU9250_S1REG                   0x29
#define MPU9250_S1ADDR                  0x28
#define MPU9250_S0CTRL                  0x27
#define MPU9250_S0REG                   0x26
#define MPU9250_S0ADDR                  0x25
#define MPU9250_PRGMSTART               0x70
#define MPU9250_MEMSTARTADDR            0x6E
#define MPU9250_BANKSEL                 0x6D
#define MPU9250_ACCEL_OFFS                 0x77
#define MPU9250_MEMRW                 0x6F
#define MPU9250_ACCELINTEL                 0x69
#define MPU9250_DMPINTSTATUS                 0x39
#define MPU9250_LPF                 0x1A
#define MPU9250_PRODID                 0x0C
#define MPU9250_GYROCFG                 0x1B
#define MPU9250_ACCELCFG                  0x1C
#define MPU9250_ACCELCFG2                 0x1D
#define MPU9250_MOTIONTHR                 0x1F
#define MPU9250_MOTIONDUR                 0x20
#define MPU9250_RAWGYRO                 0x43
#define MPU9250_RAWACCEL                 0x3B


//  Define this symbol to get debug messages

//#define  MPU_DEBUG


//  This symbol defines how many devices are supported

#define MPU_MAX_DEVICES 1

//  Call this function before using the MPU to select the correct device

int mpu_select_device(int device);

inline void get_ms(long unsigned int *timestamp)
{
    *timestamp = millis(); 
}

//  IMU hardware device defines

#define INV_X_GYRO      (0x40)
#define INV_Y_GYRO      (0x20)
#define INV_Z_GYRO      (0x10)
#define INV_XYZ_GYRO    (INV_X_GYRO | INV_Y_GYRO | INV_Z_GYRO)
#define INV_XYZ_ACCEL   (0x08)
#define INV_XYZ_COMPASS (0x01)

struct int_param_s {
    void (*cb)(void);
    unsigned short pin;
    unsigned char lp_exit;
    unsigned char active_low;
};

#define MPU_INT_STATUS_DATA_READY       (0x0001)
#define MPU_INT_STATUS_DMP              (0x0002)
#define MPU_INT_STATUS_PLL_READY        (0x0004)
#define MPU_INT_STATUS_I2C_MST          (0x0008)
#define MPU_INT_STATUS_FIFO_OVERFLOW    (0x0010)
#define MPU_INT_STATUS_ZMOT             (0x0020)
#define MPU_INT_STATUS_MOT              (0x0040)
#define MPU_INT_STATUS_FREE_FALL        (0x0080)
#define MPU_INT_STATUS_DMP_0            (0x0100)
#define MPU_INT_STATUS_DMP_1            (0x0200)
#define MPU_INT_STATUS_DMP_2            (0x0400)
#define MPU_INT_STATUS_DMP_3            (0x0800)
#define MPU_INT_STATUS_DMP_4            (0x1000)
#define MPU_INT_STATUS_DMP_5            (0x2000)

/* Set up APIs */

int mpu_init(struct int_param_s *int_param);
int mpu_init_slave(void);
int mpu_set_bypass(unsigned char bypass_on);


/* Configuration APIs */
int mpu_lp_accel_mode(unsigned char rate);
int mpu_set_int_latched(unsigned char enable);

int mpu_set_dmp_state(unsigned char enable);

int mpu_get_lpf(unsigned short *lpf);
int mpu_set_lpf(unsigned short lpf);

int mpu_get_gyro_fsr(unsigned short *fsr);
int mpu_set_gyro_fsr(unsigned short fsr);

int mpu_get_accel_fsr(unsigned char *fsr);
int mpu_set_accel_fsr(unsigned char fsr);

int mpu_get_compass_fsr(unsigned short *fsr);

int mpu_get_gyro_sens(float *sens);
int mpu_get_accel_sens(unsigned short *sens);

int mpu_get_sample_rate(unsigned short *rate);
int mpu_set_sample_rate(unsigned short rate);
int mpu_get_compass_sample_rate(unsigned short *rate);
int mpu_set_compass_sample_rate(unsigned short rate);

int mpu_get_fifo_config(unsigned char *sensors);

int mpu_get_power_state(unsigned char *power_on);
int mpu_set_sensors(unsigned char sensors);

int mpu_set_accel_bias(const long *accel_bias);

/* Data getter/setter APIs */
int mpu_get_temperature(long *data, unsigned long *timestamp);
int8_t mpu_get_accel_raw(int16_t *accX, int16_t *accY, int16_t *accZ);
int8_t mpu_get_gyro_raw(int16_t *gyrX, int16_t *gyrY, int16_t *gyrZ);
int8_t mpu_get_compass_raw(int16_t *magX, int16_t *magY, int16_t *magZ);

int mpu_get_int_status(short *status);
int mpu_reset_fifo(void);

int mpu_write_mem(unsigned short mem_addr, unsigned short length,
    unsigned char *data);
int mpu_read_mem(unsigned short mem_addr, unsigned short length,
    unsigned char *data);
int mpu_load_firmware(unsigned short length, const unsigned char *firmware,
    unsigned short start_addr, unsigned short sample_rate);

int mpu_reg_dump(void);
int mpu_read_reg(unsigned char reg, unsigned char *data);



#endif  /* #ifndef _MPU9250_I2C_H_ */
