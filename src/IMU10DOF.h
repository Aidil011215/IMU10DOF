#ifndef IMU_10DOF_H
#define IMU_10DOF_H

#include "Wire.h"
#include "Arduino.h"

#define MPU6050_ADDR                  0x68
#define QMC5883L_ADDR                 0x0D
#define BMP280_ADDR          		  0x76

#define MPU6050_SMPLRT_DIV_REGISTER   0x19
#define MPU6050_CONFIG_REGISTER       0x1a
#define MPU6050_GYRO_CONFIG_REGISTER  0x1b
#define MPU6050_ACCEL_CONFIG_REGISTER 0x1c
#define MPU6050_PWR_MGMT_1_REGISTER   0x6b
#define QMC5583L_CONFIG_REGISTER      0x0B
#define QMC5883L_MODE_REGISTER        0x09
#define BMP280_CONFIG_REGISTER        0xF5
#define BMP280_CTRL_REGISTER          0xF4

#define MPU6050_GYRO_OUT_REGISTER     0x43
#define MPU6050_ACCEL_OUT_REGISTER    0x3B
#define QMC5883L_OUT_REGISTER         0x00
#define BMP280_PRES_MSB    		      0xF7

#define BMP280_TRIM_PARAMS		      0x88
#define BMP280_RESET 			      0xE0
#define BMP280_MDICE_ID 		      0xD0
#define DEVICE_ID 					  0x58
#define RESET_CODE					  0xB6

#define RAD_2_DEG                     57.29578f // [deg/rad]
#define CALIB_OFFSET_NB_MES           200
#define TEMP_LSB_2_DEGREE             340.0f    // [bit/celsius]
#define TEMP_LSB_OFFSET               36.53f//12412.0f  

class IMUSensor {
    public:
    // INIT and BASIC FUNCTIONS
	IMUSensor(TwoWire &w = Wire);
    void begin();
    void setDeclination(int16_t degree, uint8_t minute, char dir);
    void setGyroOffsets(float x, float y, float z);
	void setAccOffsets(float x, float y, float z);
    // void setFilter(float angle = 0.005f, float bias = 0.008f, float mea = 0.08f);
    // float Filter(float newValue, float newRate, float dt, uint8_t ar);
	void calcOffsets(bool _offsetMPU = true, bool _offsetQMC = true, bool _offsetBMP = true);

    // Call variabel used for calibration sensor MPU6050
	float getGyroXoffset(){ return gyroXoffset; };
    float getGyroYoffset(){ return gyroYoffset; };
    float getGyroZoffset(){ return gyroZoffset; };
	float getAccXoffset(){ return accXoffset; };
	float getAccYoffset(){ return accYoffset; };
	float getAccZoffset(){ return accZoffset; };

    //Call variabel for value measurement
    float getAngleX(){ return angleX; };
    float getAngleY(){ return angleY; };
    float getAngleZ(){ return angleZ; };
    float getAzimuth(){ return Azimuth;};
    float getHeading(){ return Heading;};
    float getAltitude(){ return Altitude;};
    float getPressure(){ return Pressure;};
    float getTemperature(){ return temp / 1.14;};

    // Call function for update value measurement IMU Sensors
    void update();

	bool upsideDownMounting = false;

    private:
    TwoWire *wire;
	void calcDataMPU(); // user should better call function for update measurement sensors MPU6050
	void calcDataQMC(); // user should better call function for update measurement sensors QMC5883L 
	void calcDataBMP(); // user should better call function for update measurement sensors BMP280
    void readMPU(uint8_t reg, int bitData);
    void readQMC(uint8_t reg, int bitData);
    void readBMP(uint8_t reg, uint8_t* data, int16_t bitData);
	uint8_t writeByte(uint8_t add, uint8_t reg, uint8_t data);
    uint8_t readByte(uint8_t add, uint8_t reg);
    float gyroXoffset, gyroYoffset, gyroZoffset;
	float accXoffset, accYoffset, accZoffset;
    float temp, accX, accY, accZ, gyroX, gyroY, gyroZ;
    float angleAccX, angleAccY, angleX, angleY, angleZ;
    uint32_t preInterval;
    float Azimuth, Heading, _declination, startHeading;
    float TempB, Pressure, Altitude, startAltitude;
	// float Q_angle, Q_bias, R_measure;
	// float K_angle[3], K_bias[3], K_rate[3], Sum[3], err[3];
	// float P[4][3], K[2][3];
    struct {
        uint16_t dig_T1;
        int16_t  dig_T2;
        int16_t  dig_T3;
        uint16_t dig_P1;
        int16_t  dig_P2;
        int16_t  dig_P3;
        int16_t  dig_P4;
        int16_t  dig_P5;
        int16_t  dig_P6;
        int16_t  dig_P7;
        int16_t  dig_P8;
        int16_t  dig_P9;
	} params;
};

extern IMUSensor IMU;
#endif