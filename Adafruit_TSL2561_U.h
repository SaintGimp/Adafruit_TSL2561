/**************************************************************************/
/*! 
    @file     Adafruit_TSL2561.h
    @author   K. Townsend (Adafruit Industries)

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2013, Adafruit Industries
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/
#ifndef _TSL2561_H_
#define _TSL2561_H_

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define TSL2561_VISIBLE 2                   // channel 0 - channel 1
#define TSL2561_INFRARED 1                  // channel 1
#define TSL2561_FULLSPECTRUM 0              // channel 0

// I2C address options
#define TSL2561_ADDR_LOW          (0x29)
#define TSL2561_ADDR_FLOAT        (0x39)    // Default address (pin left floating)
#define TSL2561_ADDR_HIGH         (0x49)

// Lux calculations differ slightly for CS package
//#define TSL2561_PACKAGE_CS
#define TSL2561_PACKAGE_T_FN_CL

#define TSL2561_COMMAND_BIT       (0x80)    // Must be 1
#define TSL2561_CLEAR_BIT         (0x40)    // Clears any pending interrupt (write 1 to clear)
#define TSL2561_WORD_BIT          (0x20)    // 1 = read/write word (rather than byte)
#define TSL2561_BLOCK_BIT         (0x10)    // 1 = using block read/write

#define TSL2561_CONTROL_POWERON   (0x03)
#define TSL2561_CONTROL_POWEROFF  (0x00)

// Auto-gain thresholds
#define TSL2561_AGC_THI_13MS      (4850)    // Max value at Ti 13ms = 5047
#define TSL2561_AGC_TLO_13MS      (100)
#define TSL2561_AGC_THI_101MS     (36000)   // Max value at Ti 101ms = 37177
#define TSL2561_AGC_TLO_101MS     (200)
#define TSL2561_AGC_THI_402MS     (63000)   // Max value at Ti 402ms = 65535
#define TSL2561_AGC_TLO_402MS     (500)

// Clipping thresholds
#define TSL2561_CLIPPING_13MS     (4900)
#define TSL2561_CLIPPING_101MS    (37000)
#define TSL2561_CLIPPING_402MS    (65000)

enum
{
  TSL2561_REGISTER_CONTROL          = 0x00,
  TSL2561_REGISTER_TIMING           = 0x01,
  TSL2561_REGISTER_THRESHHOLDL_LOW  = 0x02,
  TSL2561_REGISTER_THRESHHOLDL_HIGH = 0x03,
  TSL2561_REGISTER_THRESHHOLDH_LOW  = 0x04,
  TSL2561_REGISTER_THRESHHOLDH_HIGH = 0x05,
  TSL2561_REGISTER_INTERRUPT        = 0x06,
  TSL2561_REGISTER_CRC              = 0x08,
  TSL2561_REGISTER_ID               = 0x0A,
  TSL2561_REGISTER_CHAN0_LOW        = 0x0C,
  TSL2561_REGISTER_CHAN0_HIGH       = 0x0D,
  TSL2561_REGISTER_CHAN1_LOW        = 0x0E,
  TSL2561_REGISTER_CHAN1_HIGH       = 0x0F
};

typedef enum
{
  TSL2561_INTEGRATIONTIME_13MS      = 0x00,    // 13.7ms
  TSL2561_INTEGRATIONTIME_101MS     = 0x01,    // 101ms
  TSL2561_INTEGRATIONTIME_402MS     = 0x02     // 402ms
}
tsl2561IntegrationTime_t;

typedef enum
{
  TSL2561_GAIN_1X                   = 0x00,    // No gain
  TSL2561_GAIN_16X                  = 0x10,    // 16x gain
}
tsl2561Gain_t;

class Adafruit_TSL2561_Unified : public Adafruit_Sensor {
 public:
  Adafruit_TSL2561_Unified(uint8_t addr, int32_t sensorID = -1);
  boolean begin(void);
  
  /* TSL2561 Functions */
  void enableAutoRange(bool enable);
  void setIntegrationTime(tsl2561IntegrationTime_t time);
  void setGain(tsl2561Gain_t gain);
  void getLuminosity (uint16_t *broadband, uint16_t *ir);
  float calculateLux(uint16_t broadband, uint16_t ir);
  
  /* Unified Sensor API Functions */  
  void getEvent(sensors_event_t*);
  void getSensor(sensor_t*);

 private:
  int8_t _addr;
  boolean _tsl2561Initialised;
  boolean _tsl2561AutoGain;
  tsl2561IntegrationTime_t _tsl2561IntegrationTime;
  tsl2561Gain_t _tsl2561Gain;
  int32_t _tsl2561SensorID;
  
  void     enable (void);
  void     disable (void);
  void     write8 (uint8_t reg, uint32_t value);
  uint8_t  read8 (uint8_t reg);
  uint16_t read16 (uint8_t reg);
  void     getData (uint16_t *broadband, uint16_t *ir);
};
#endif
